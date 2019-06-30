#include "SendImage.h"

void datacleanup (void *data, void *hint) {
  //  free (data);
}

SendImage::SendImage():Tool(){}


bool SendImage::Initialise(std::string configfile, DataModel &data){

  if(configfile!="")  m_variables.Initialise(configfile);
  //m_variables.Print();

  m_data= &data;

  zmq::socket_t Ireceive (*m_data->context, ZMQ_PUSH);
  Ireceive.connect("inproc://ServicePublish");

  boost::uuids::uuid m_UUID;
  m_UUID = boost::uuids::random_generator()();
  std::stringstream test;
  test<<"Add "<< "MonitorData "<<m_UUID<<" 55555 "<<"0";
  zmq::message_t send(test.str().length());
  snprintf ((char *) send.data(), test.str().length() , "%s" ,test.str().c_str()) ;
  Ireceive.send(send);

  ISend=new  zmq::socket_t(*m_data->context, ZMQ_PUB);
  ISend->bind("tcp://*:55555");
  
  return true;
}


bool SendImage::Execute(){
  
  if(m_data->capture){
    
    zmq::message_t message(7);
    
    snprintf ((char *) message.data(), 7 , "%s" ,"Camera") ;
    ISend->send(message, ZMQ_SNDMORE);

    zmq::message_t pic(m_data->data, m_data->length, datacleanup);
    ISend->send(pic);
    //std::ofstream file ( "picture.png",std::ios::binary );
    //file.write ( ( char* ) m_data->data,   m_data->length );
    //file.close();
    
    
  }
  
  return true;
}


bool SendImage::Finalise(){

  zmq::socket_t Ireceive (*m_data->context, ZMQ_PUSH);
  Ireceive.connect("inproc://ServicePublish");
  std::stringstream test;
  test<<"Delete "<< "MonitorData ";
  zmq::message_t send(test.str().length()+1);
  snprintf ((char *) send.data(), test.str().length()+1 , "%s" ,test.str().c_str()) ;
  Ireceive.send(send);

  delete ISend;
  ISend=0;
  
  
  return true;
}
