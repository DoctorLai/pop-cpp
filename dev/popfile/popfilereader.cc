/**
 * File : popfilemanager.cc
 * Author : Valentin Clement
 * Description : Implementation of the parallel object POPFileReader. This object is in charge of asynchronous read of
 *strip.
 * Creation date : 05/04/2012
 *
 * Change Log:
 * Author       Date            Description
 * clementval   05.04.2012  Creation of this file
 */

#include "popfilereader.ph"
#include "popfilemanager.ph"
#include "popfilebuffer.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * POPFileReader constructor
 */
POPFileReader::POPFileReader() {
    cout << "[POPFILEREADER] Created on " << POPGetHost() << popcendl;
    popfilebuffer_ref = new POPFileBuffer();
    pt_read_locker == NULL;
}

/**
 * POPFileReader destructor
 */
POPFileReader::~POPFileReader() {
    sem_unlink(reader_semname.c_str());
}

/**
 * Set the identifier of this POPFileReader and initialize its semaphore
 * @param value The identifier value
 * @return void
 */
void POPFileReader::set_id(int value) {
    identifier = value;
    if (pt_read_locker == NULL) {
        std::stringstream semname;
        semname << "/pfrs";
        semname << identifier;
        reader_semname = semname.str();
        sem_unlink(reader_semname.c_str());
        pt_read_locker = sem_open(reader_semname.c_str(), O_CREAT | O_EXCL, 0600, 1);
        sem_wait(pt_read_locker);
    }
}

/**
 * Asynchronous read of the strip. Read the data from the strip and store them in the internal buffer
 * @param start pointer to start reading
 * @param offset    Number of byte to read from the start point
 * @return void
 */
void POPFileReader::read_in_strip(long start, long offset) {
    // cout << "[POPFILEREADER] Read in strip " << strip_path.GetString() << " from:" << start << " to:" << start+offset
    // << popcendl;
    std::string data = pfm_ref->readFromStrip(strip_path, start, offset);
    // cout << "[POPFILEREADER] Read from strip (bytes) " << strlen(data.GetString()) << popcendl;
    popfilebuffer_ref->add_data(data);
    sem_post(pt_read_locker);
}

/**
 * Read data in the internal buffer
 * @param size Number of byte to read
 * @return Data read as a std::string object
 */
std::string POPFileReader::read_current_buffer(long size) {
    while (popfilebuffer_ref->get_size_input_data() < size) {
        cout << "Before lock: id = " << identifier << " data = " << popfilebuffer_ref->get_size_input_data() << "Size"
             << size << popcendl;
        sem_wait(pt_read_locker);
    }
    std::string data;
    if (size != -1) {
        data = popfilebuffer_ref->buffer_get(size).c_str();
    } else {
        data = popfilebuffer_ref->buffer_get_offset().c_str();
    }
    cout << "[POPFILEREADER] Sent data size " << strlen(data.GetString()) << popcendl;
    return data;
}

/**
 * Set the offset of the associated strip
 * @param offset The offset of the associated strip
 * @return void
 */
void POPFileReader::set_offset(long offset) {
    pfr_offset = offset;
    popfilebuffer_ref->set_capacity(pfr_offset);
}

/**
 * Save the local PFM accesspoint
 * @param ap    The access point of the associated PFM
 * @return void
 */
void POPFileReader::set_pfm_accesspoint(pop_accesspoint ap) {
    pfm_ap = ap;
    pfm_ref = new POPFileManager(pfm_ap);
}

/**
 * Get the local PFM accesspoint
 * @return Access Point of the POPFileManager
 */
pop_accesspoint POPFileReader::get_pfm_accesspoint() {
    return pfm_ap;
}

/**
 * Save the associated strip path
 * @param path The absolute strip file path associated with this reader
 * @return void
 */
void POPFileReader::set_strip_path(std::string path) {
    strip_path = path;
}

/**
 * Get the associated strip path
 * @return the absolute strip file path associated with this reader
 */
std::string POPFileReader::get_strip_path() {
    return strip_path;
}

// Specific POP-C++ annotation
@pack(POPFileReader);