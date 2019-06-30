#ifndef CameraReceive_H
#define CameraReceive_H

#include <string>
#include <iostream>

#include "Tool.h"

#include "boost/date_time/gregorian/gregorian.hpp"
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/chrono.hpp>
#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp>         // streaming operators etc.

/**
 * \class CameraReceive
 *
 * This is a balnk template for a Tool used by the script to generate a new custom tool. Please fill out the descripton and author information.
*
* $Author: B.Richards $
* $Date: 2019/05/28 10:44:00 $
* Contact: b.richards@qmul.ac.uk
*/
class CameraReceive: public Tool {


 public:

  CameraReceive(); ///< Simple constructor
  bool Initialise(std::string configfile,DataModel &data); ///< Initialise Function for setting up Tool resorces. @param configfile The path and name of the dynamic configuration file to read in. @param data A reference to the transient data class used to pass information between Tools.
  bool Execute(); ///< Executre function used to perform Tool perpose. 
  bool Finalise(); ///< Finalise funciton used to clean up resorces.


 private:

  int UpdateCameraSources();
  int sources;
  zmq::socket_t* CameraReceiver;
  boost::posix_time::ptime last;
  boost::posix_time::time_duration period;
  zmq::pollitem_t items[1];
  std::map<std::string,Store*> connections;
  



};


#endif
