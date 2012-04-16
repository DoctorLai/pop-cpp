/**
 * File : popcloner.cc
 * Author : Valentin Clement
 * Description : Implementation of the parallel object POPCloner (POP-C++ Global Services). This object is responsible of the 
 *               VM cloning.
 * Creation date : 2010/11/12
 * 
 * Modifications : 
 * Authors		Date			Comment
 * clementval	2010/11/12  Creation of this file
 */

#include "popfilemanager.ph"
#include "popc_search_node.ph"

#include <stdlib.h>
#include <sstream>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <stdarg.h>

POPFileManager::POPFileManager(const POPString &challenge, bool deamon, POPString host) : paroc_service_base(challenge) {
	popfile_log("[POPFILEMANAGER] POPFileManager created.");

	if(deamon) Start();
}

POPFileManager::~POPFileManager(){

}

bool POPFileManager::createStrip()
{
	popfile_log("[POPFILEMANAGER] Call on createStrip");	
	return true;
}
   
   
/**
 * POPFile: clementval
 * Write data to a strip
 * @param stringName	absloute path to the strip
 * @param data			Actual data to write to the strip
 */
void POPFileManager::writeToStrip(POPString stringName, POPString data)
{
	std::ofstream strip;
  	strip.open (stringName.GetString());
  	if(strip.is_open()){
  		strip << data.GetString();
 		strip.close();
  	} else {
  		popfile_log("[POPFILEMANAGER] Attempt to wrtie data to %s failed", stringName.GetString());
  	}
}




/**
 * POPFile: clementval
 * Save the accesspoint of POPSearchNode
 */
void POPFileManager::setPSNAccessPoint(paroc_accesspoint ap)
{
	psn_ap = ap;
	POPCSearchNode psn(psn_ap);
	POPString neighborsList = psn.getNeighborsAsString();
	popfile_log("[POPFILEMANAGER] neighbors: %s", neighborsList.GetString());
	
}


void POPFileManager::findResourcesForStrip(int nb){
	popfile_log("[POPFILEMANAGER] Look for %d nodes for strips", nb);
	
		
}






/**
 * ViSaG : clementval
 * Method used to write log
 * @param String with format
 */
int popfile_log(const char *format,...)
{
	char *tmp=getenv("POPC_TEMP");
	char logfile[256];
	if (tmp!=NULL) sprintf(logfile,"%s/popfile_log",tmp);
	else strcpy(logfile, "/tmp/popfile.log");

	FILE *f=fopen(logfile,"a");
	if (f==NULL) return 1;
	time_t t=time(NULL);
	fprintf(f,"%s", ctime(&t));
	va_list ap;
	va_start(ap, format);
	vfprintf(f, format, ap);
	fprintf(f,"%s","\n");
	va_end(ap);
	fclose(f);
	return 0;
}
