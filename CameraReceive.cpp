#include "CameraReceive.h"

CameraReceive::CameraReceive():Tool(){}


bool CameraReceive::Initialise(std::string configfile, DataModel &data){

  if(configfile!="")  m_variables.Initialise(configfile);
  //m_variables.Print();

  m_data= &data;

  std::string outpath="";
  m_variables.Get("OutPath",outpath);
  m_data->CStore.Set("OutPath",outpath);

  CameraReceiver= new zmq::socket_t(*m_data->context, ZMQ_SUB);
  CameraReceiver->setsockopt(ZMQ_SUBSCRIBE, "", 0);

  items[0].socket = *CameraReceiver;
  items[0].fd = 0;
  items[0].events = ZMQ_POLLIN;
  items[0].revents =0;


  sources=UpdateCameraSources();

  last= boost::posix_time::ptime(boost::posix_time::second_clock::local_time());
  period =boost::posix_time::time_duration(0,0,5,0);

  return true;
}


bool CameraReceive::Execute(){

  boost::posix_time::ptime current(boost::posix_time::second_clock::local_time());
  boost::posix_time::time_duration duration(current - last);
  if(duration>period){
    last=current;
    sources=UpdateCameraSources();
  }

  if(sources>0){
    zmq::poll(&items[0], 1, 100);

    if ((items [0].revents & ZMQ_POLLIN)) {
      zmq::message_t command;
      CameraReceiver->recv(&command);

      std::istringstream tmp(static_cast<char*>(command.data()));

      if (tmp.str()=="Camera"){

	zmq::message_t picture;
	CameraReceiver->recv(&picture);
	
	std::ofstream file ( "picture.png",std::ios::binary );
	file.write ( ( char* ) picture.data(),   picture.size() );
	file.close();
	
	
      }

    }
      
  }
  
  return true;
}


bool CameraReceive::Finalise(){

  delete CameraReceiver;
  CameraReceiver=0;

  for ( std::map<std::string,Store*>::iterator it=connections.begin(); it!=connections.end(); ++it){
    delete it->second;
    it->second=0;
  }

  connections.clear();

  m_data->CStore.Remove("State");

  m_data->Stores["CCData"]->Remove("FileData");
  m_data->Stores["CCData"]->Remove("Single");
  m_data->Stores.clear();
  
  
  return true;
}

int CameraReceive::UpdateCameraSources(){

  boost::uuids::uuid m_UUID=boost::uuids::random_generator()();
  long msg_id=0;

  zmq::socket_t Ireceive (*m_data->context, ZMQ_DEALER);
  Ireceive.connect("inproc://ServiceDiscovery");


  zmq::message_t send(4);
  snprintf ((char *) send.data(), 4 , "%s" ,"All") ;


  Ireceive.send(send);


  zmq::message_t receive;
  Ireceive.recv(&receive);
  std::istringstream iss(static_cast<char*>(receive.data()));

  int size;
  iss>>size;

  for(int i=0;i<size;i++){

    Store *service = new Store;

    zmq::message_t servicem;
    Ireceive.recv(&servicem);

    std::istringstream ss(static_cast<char*>(servicem.data()));
    service->JsonParser(ss.str());

    std::string type;
    std::string uuid;
    service->Get("msg_value",type);
    service->Get("uuid",uuid);

    if(type == "MonitorData" && connections.count(uuid)==0){
      connections[uuid]=service;
      std::string ip;
      std::string port;
      service->Get("ip",ip);
      service->Get("remote_port",port);
      std::string tmp="tcp://"+ ip +":"+port;
      CameraReceiver->connect(tmp.c_str());
    }
    else{
      delete service;
      service=0;
    }


  }

  return connections.size();
}
