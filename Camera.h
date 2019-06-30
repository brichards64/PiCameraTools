#ifndef Camera_H
#define Camera_H

#include <string>
#include <iostream>

#include "Tool.h"
#include <ctime>
#include <fstream>
#include <cstdlib>
#include <iostream>
#include <raspicam_still.h>
#include <unistd.h>
//#include <pngwriter.h>
#include <sstream>

/**
 * \class Camera
 *
 * This is a balnk template for a Tool used by the script to generate a new custom tool. Please fill out the descripton and author information.
*
* $Author: B.Richards $
* $Date: 2019/05/28 10:44:00 $
* Contact: b.richards@qmul.ac.uk
*/
class Camera: public Tool {


 public:

  Camera(); ///< Simple constructor
  bool Initialise(std::string configfile,DataModel &data); ///< Initialise Function for setting up Tool resorces. @param configfile The path and name of the dynamic configuration file to read in. @param data A reference to the transient data class used to pass information between Tools.
  bool Execute(); ///< Executre function used to perform Tool perpose. 
  bool Finalise(); ///< Finalise funciton used to clean up resorces.


 private:

  raspicam::RaspiCam_Still* camera;



};


#endif
