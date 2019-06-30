#include "Camera.h"

Camera::Camera():Tool(){}


bool Camera::Initialise(std::string configfile, DataModel &data){

  if(configfile!="")  m_variables.Initialise(configfile);
  //m_variables.Print();

  m_data= &data;

  camera = new raspicam::RaspiCam_Still;
  if ( !camera->open()) {std::cout<<"Error opening camera"<<std::endl;return false;}
  usleep(3); 
  camera->setEncoding(raspicam::RASPICAM_ENCODING_PNG);
  m_data->data=0;
  
  
  return true;
}


bool Camera::Execute(){
     
  if(m_data->capture){
    
    m_data->length = camera->getImageBufferSize(); // Header + Image Data + Padding
    delete m_data->data;
    m_data->data=0;
    m_data->data=new unsigned char[m_data->length];
    camera->grab_retrieve(m_data->data, m_data->length);
        
  }
 
  return true;
}


bool Camera::Finalise(){

  delete camera;
  camera=0;

  delete  m_data->data;
  m_data->data=0;
  
  return true;
}
