#include "Scheduler.h"

Scheduler::Scheduler():Tool(){}


bool Scheduler::Initialise(std::string configfile, DataModel &data){

  if(configfile!="")  m_variables.Initialise(configfile);
  //m_variables.Print();

  m_data= &data;

  if(!m_variables.Get("interval_sec",interval_sec)) interval_sec=60;

  last= boost::posix_time::second_clock::local_time();
  period = boost::posix_time::time_duration(0, 0, interval_sec, 0);
  
  
  m_data->capture=false;
  
  return true;
}


bool Scheduler::Execute(){

  m_data->capture=false;
  
  boost::posix_time::ptime current= boost::posix_time::second_clock::local_time();
  boost::posix_time::time_duration lapse(period - (current - last));
  
  if (lapse.is_negative()){
    m_data->capture=true;
    last=current;
  }
  else usleep(500);
  
  
  return true;
}


bool Scheduler::Finalise(){

  return true;
}
