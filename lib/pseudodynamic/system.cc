/**
 *
 * Copyright (c) 2005-2012 POP-C++ project - GRID & Cloud Computing group, University of Applied Sciences of western Switzerland.
 * http://gridgroup.hefr.ch/popc
 *
 * @author Tuan Anh Nguyen
 * @date 2005/01/01
 * @brief System stuffs and declarations used by the runtime.
 *
 *
 * Modifications :
 * Authors    Date      Comment
 * L.Winkler  2008-2009   for version 1.3
 * P.Kuonen   02/2010     (GetHost, getIp, add POPC_Host_Name, ...) for version 1.3m (see comments 1.3m)
 * P.Kuonen   02/2011      define default IP for version 1.3.1m (see comments 1.3.1m)
 * P.Kuonen   25/3/2011    Cosmetic changes
 */

/*
  Deeply need refactoring:
    POPC_System instead of pop_system
 */
#include "popc_intface.h"

//#include <stdio.h>
//#include <netdb.h>
//#include <unistd.h>
//#include <sys/socket.h>
//#include <netinet/in.h>
//#include <arpa/inet.h>
//#include <ctype.h>


#include "pop_system_mpi.h"
#include "pop_buffer_factory_finder.h"
#include "paroc_utils.h"
#include "pop_combox_factory.h"
#include "paroc_exception.h"
#include "paroc_od.h"

pop_accesspoint pop_system::appservice;
pop_accesspoint pop_system::jobservice;
int pop_system::pop_current_local_address;

int pop_system::popc_local_mpi_communicator_rank;

int pop_system_mpi::current_free_process;
bool pop_system_mpi::is_remote_object_process;
bool pop_system_mpi::mpi_has_to_take_lock;
paroc_condition pop_system_mpi::mpi_unlock_wait_cond;
paroc_condition pop_system_mpi::mpi_go_wait_cond;


std::string pop_system::platform;
std::ostringstream pop_system::_popc_cout;

//V1.3m
std::string pop_system::POPC_HostName;
#define LOCALHOST "localhost"
//End modif

AppCoreService *pop_system::mgr=NULL;
std::string pop_system::challenge;

pop_system::pop_system() {
    pop_combox_factory::GetInstance();
    pop_buffer_factory_finder::GetInstance();
    char *tmp = getenv("POPC_PLATFORM");
    if(tmp != NULL) {
        platform = tmp;
    } else {
        char str[128];
#ifndef POPC_ARCH
        char arch[64], sysname[64];
#ifndef __WIN32__
        // popc_sysinfo(SI_SYSNAME,sysname,64);
        // popc_sysinfo(SI_ARCHITECTURE,arch,64);
#endif
        sprintf(str,"%s-%s",sysname,arch);
#else
#ifndef __WIN32__
        strcpy(str,POPC_ARCH);
#else
        strcpy(str, "win32");
#endif
#endif
        platform = str;
    }
    POPC_HostName = std::string("");
}


pop_system::~pop_system() {
#ifndef DEFINE_UDS_SUPPORT
    /*if (mgr!=NULL)
    {
      Finalize(false);
      delete mgr;
    }
    mgr=NULL;*/
#endif

    pop_combox_factory *pf=pop_combox_factory::GetInstance();
    pop_buffer_factory_finder *bf=pop_buffer_factory_finder::GetInstance();
    delete pf;
    delete bf;
}



// V1.3m
// Try to determine the Host Name of the machine and put it in POPC_Host_Name
// IF env. variable POPC_HOST is defined THEN use POPC_HOST
// ELSE IF try to use gethostname() and possibly env. variable POPC_DOMAIN
// ELSE call GetIP()
//----------------------------------------------------------------------------
std::string pop_system::GetHost() {
    if(POPC_HostName.empty()) {
        char str[128];
        char *t=getenv("POPC_HOST");
        if(t==NULL || *t==0) {
            popc_gethostname(str,127);
            if(strchr(str,'.')==NULL || strstr(str,".local\0")!=NULL) {
                int len=strlen(str);
                char *domain=getenv("POPC_DOMAIN");
                if(domain!=NULL && domain!=0) {
                    str[len]='.';
                    strcpy(str+len+1,domain);
                    POPC_HostName = str;
                } else { //(domain!=NULL && domain!=0)
                    POPC_HostName = GetIP();
                }
            } else { //(strchr(str,'.')==NULL || strstr(str,".local\0")!=NULL)
                POPC_HostName = str;
            }
        } else { //(t==NULL || *t==0)
            POPC_HostName=t;
        }
    }
    LOG_DEBUG("GetHost returns %s", POPC_HostName.c_str());
    return POPC_HostName;
}


// V1.3m
// Try to determine the IP address of the machine.
// If fail return the localhost IP = LOCALHOST
//------------------------------------------------------
std::string pop_system::GetIP() {
#ifndef __WIN32__
    std::string iface,ip;
    char* tmp;
    ip = std::string(LOCALHOST);

    tmp=getenv("POPC_IP");
    if(tmp!=NULL) {     // Env. variable POPC_IP is defined
        ip=tmp;
    } else {           //Env. variable POPC_IP is not defined
        tmp=getenv("POPC_IFACE");
        if(tmp!=NULL) { // Env. Variable POP_IFACE is defined
            iface=tmp;    // Try to determine IP from network interface name
            if(!(GetIPFromInterface(iface,ip))) {
                // Not found
                setenv("POPC_IP",LOCALHOST, 0); // V1.3.1m define LOCALHOST as IP
                LOG_WARNING("Cannot find an IP for interface %s, using '%s' as IP address.",iface.c_str(), LOCALHOST);
            }
        } else { // Env. Variable POP_IFACE is not defined
            iface=GetDefaultInterface();
            if(!iface.empty()) {
                if(!(GetIPFromInterface(iface,ip))) {
                    setenv("POPC_IP",LOCALHOST, 0); // V1.3.1m define LOCALHOST as IP
                    LOG_WARNING("host IP not found, using '%s' as IP address.",LOCALHOST);
                }
            } else {
                setenv("POPC_IP",LOCALHOST, 0); // V1.3.1m define LOCALHOST as IP
                LOG_WARNING("no default network interface found, using '%s' as IP address.", LOCALHOST);
            }
        }
    }
    return ip;
#else
    char ipaddr[64];
    char hostname[256];
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    if(popc_gethostname(hostname,256)!=0) {
        return std::string("127.0.0.1");
    }
    struct hostent *hp=gethostbyname(hostname);
    unsigned ip=(unsigned(127)<<24)+1;

    if(hp==NULL || *(hp->h_addr_list)==NULL) {
        return std::string("127.0.0.1");
    }

    char **p=hp->h_addr_list;
    while(*p!=NULL) {
        memcpy(&ip,*p, sizeof(unsigned));
        ip=ntohl(ip);
        if(ip!=(unsigned(127)<<24)+1) {
            break;
        }
        p++;
    }
    int n1,n2,n3,n4;
    n1=ip & 255;
    ip=ip>>8;

    n2=ip & 255;
    ip=ip>>8;

    n3=ip & 255;
    ip=ip>>8;

    n4=ip & 255;
    sprintf(ipaddr,"%d.%d.%d.%d",n4,n3,n2,n1);
    WSACleanup();
    return ipaddr;
#endif
}

int pop_system::GetIP(const char *hostname, int *iplist, int listsize) {
    /* This method should normally not be used to return more than one ip*/
    assert(listsize==1);
#ifdef __WIN32__
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif
    struct hostent *hp;
    int n;
    char **p;
    int addr;
    if((int)(addr = popc_inet_addr(hostname)) == -1) {
        hp=gethostbyname(hostname);
    } else {
        if(listsize>0) {
            *iplist=popc_ntohl(addr);
            return 1;
        }
        return 0;
    }
    if(hp==NULL) {
        return 0;
    }
    n=0;
    for(p = hp->h_addr_list; *p != 0 && n<listsize; p++) {
        memcpy(iplist, *p, sizeof(int));
        *iplist=popc_ntohl(*iplist);
        if(*iplist==(int(127)<<24)+1) {
            continue;
        }
        n++;
        iplist++;
    }
    popc_endhostent();
    if(n==0) {
        n=1;
    }
#ifdef __WIN32__
    WSACleanup();
#endif
    return n;
}


int pop_system::GetIP(int *iplist, int listsize) {
    assert(listsize==1); /* This method cannot return more than one ip*/
    char* parocip=popc_strdup(GetIP().c_str());
    int n;
    int addr;
    std::vector<std::string> tokens;
    popc_tokenize_r(tokens, parocip," \n\r\t");
    n=0;
    for(auto tok : tokens) {
        if(!n<listsize)
            break;
        if((int)(addr = popc_inet_addr(tok.c_str())) != -1) {
            *iplist=popc_ntohl(addr);
            iplist++;
            n++;
        }
    }
    return n;
}

std::string pop_system::GetDefaultInterface() {
    char buff[1024], iface[16], net_addr[128];
    //char flags[64], mask_addr[128], gate_addr[128];
    //int iflags, metric, refcnt, use, mss, window, irtt;
    bool found=false;
    FILE *fp = fopen("/proc/net/route", "r");

    if(fp != NULL) { //else
        while(fgets(buff, 1023, fp) && !found) {
            //num = sscanf(buff, "%16s %128s %128s %X %d %d %d %128s %d %d %d",
            //       iface, net_addr, gate_addr, &iflags, &refcnt, &use, &metric, mask_addr, &mss, &window, &irtt);
            int num = sscanf(buff, "%16s %128s",iface, net_addr);
            if(num < 2) {
                paroc_exception::paroc_throw("GetDefaultInterface failed: num < 2");
            }
            // LOG_DEBUG("iface %s, net_addr %s, gate_addr %s, iflags %X, &refcnt %d, &use %d, &metric %d, mask_addr %s, &mss %d, &window %d, &irtt %d\n\n",iface, net_addr, gate_addr,iflags, refcnt, use, metric, mask_addr, mss, window, irtt);

            if(!strcmp(net_addr,"00000000")) {
                LOG_DEBUG("Default gateway : %s", iface);
                found=true;
            }
        }
        fclose(fp);
    }
    if(!found) {
        iface[0] = '\0';    // if not found iface = ""
    }
    return std::string(iface);
}

// TODO LW: Should probably be in intface
bool pop_system::GetIPFromInterface(std::string &iface, std::string &str_ip) {
#ifndef __WIN32__
    (void) iface;
    (void) str_ip;
    /*struct ifaddrs *addrs, *iap;
    struct sockaddr_in *sa;
    char str_ip_local[32];

    getifaddrs(&addrs);

    LOG_DEBUG("Looking for interface: %s --->",iface.c_str());
    for (iap = addrs; iap != NULL; iap = iap->ifa_next){
        LOG_DEBUG("name=%s, addr=%p, flag=%d (%d), familly=%d (%d)",iap->ifa_name, iap->ifa_addr, iap->ifa_flags, IFF_UP, iap->ifa_addr->sa_family, AF_INET);
      if ( iap->ifa_addr &&
           (iap->ifa_flags & IFF_UP) &&
           (iap->ifa_addr->sa_family == AF_INET) &&
                !strcmp(iap->ifa_name, iface.c_str())) {
        sa = (struct sockaddr_in *)(iap->ifa_addr);
        inet_ntop(iap->ifa_addr->sa_family,
                  (void *)&(sa->sin_addr),
                  str_ip_local,
                  sizeof(str_ip_local) );
            LOG_DEBUG("The IP of interface %s is %s",iap->ifa_name,str_ip_local);
        str_ip=str_ip_local;
        freeifaddrs(addrs);
        return true;
      }
    }
    freeifaddrs(addrs);*/
    return false;
#else
    (void) iface;
    (void) str_ip;
#endif
}


/**
 * Initialize the base system to run a POP-C++ application
 * @param   argc    Number of arguments (passed from the main)
 * @param   argv    Arguments (passed from the main)
 * @return  TRUE if the system is initialized. FALSE in any others cases.
 */
bool pop_system::Initialize(int *argc,char ***argv) {
    // Get access point address of the Job Manager
    const char *info=paroc_utils::checkremove(argc,argv,"-jobservice=");
    if(info==NULL) {
        LOG_ERROR("missing -jobservice argument");
        return false;
    }
    pop_system::jobservice.SetAccessString(info);
    pop_system::jobservice.SetAsService();

    // Get path of the application service executable
    const char *codeser=paroc_utils::checkremove(argc,argv,"-appservicecode=");
    // const char *proxy=paroc_utils::checkremove(argc,argv,"-proxy=");

    // Check if need to run on local node only
    if(paroc_utils::checkremove(argc,argv,"-runlocal")) {
        paroc_od::defaultLocalJob=true;
    }

    // Get application service contact address
    const char *appcontact = paroc_utils::checkremove(argc,argv,"-appservicecontact=");

    if(codeser==NULL && appcontact==NULL) {
        LOG_ERROR("missing -appservicecontact=... or -appservicecode=... argument");
        return false;
    }
    try {
        if(appcontact == NULL) {
            char url[1024];
            strcpy(url, codeser);

#ifndef DEFINE_UDS_SUPPORT
            // Creating application services
            //mgr = CreateAppCoreService(url);
            // LOG_DEBUG("POP-C++ Application Service created %s", mgr->GetAccessPoint().GetAccessString());
#endif
        } else {
            challenge="";
            pop_accesspoint app;
            app.SetAccessString(appcontact);
            app.SetAsService();
#ifndef DEFINE_UDS_SUPPORT
           //mgr = new AppCoreService(app);
#endif
        }
        //pop_system::appservice=mgr->GetAccessPoint();
        pop_system::appservice.SetAsService();
     } catch(std::exception &e) {
        LOG_WARNING("Exception occurs in pop_system::Initialize: %s", e.what());
        Finalize(false);
        return false;
    }

    char *codeconf = paroc_utils::checkremove(argc,argv,"-codeconf=");
    (void) codeconf; // Added this to avoid warning

    LOG_DEBUG_IF(codeconf==NULL,"No code config file");

    /*if (codeconf!=NULL && !paroc_utils::InitCodeService(codeconf,mgr))
    {
    return false;
    }
    else return true; */

    LOG_DEBUG("SYSTEM: Init code service");
    //bool ret = !(codeconf != NULL && !paroc_utils::InitCodeService(codeconf,mgr));
    LOG_DEBUG("SYSTEM: Init code service done");
    return false;
}

void pop_system::Finalize(bool /*normalExit*/) {
#ifndef DEFINE_UDS_SUPPORT
    //LOG_DEBUG("Finalize the application %s", normalExit ? "true" : "false");
    /*  if (mgr != NULL) {
        //printf("mgr not null\n");
        try {
            if(normalExit) {
            // Wait all object to be terminated before exiting
            int timeout = 1;
            int oldcount = 0, count;
            int loop = 0;
            while ((count = mgr->CheckObjects()) > 0){
              if (timeout < 1800 && oldcount == count){
                        // sleep an increasing amount of time
                timeout = timeout * 4/3;
                loop++;
                if (loop % 10 == 0) {
                  timeout += 1;
                }
              } else {
                loop = 0;
                timeout = 1;
              }
                    LOG_INFO("%d parallel objects remain. Awaiting %d s. Type Ctrl+C to kill all remaining objects", count, timeout);
                    popc_sleep(timeout);
              oldcount = count;
            }
          } else {
	            LOG_INFO("Main routine did not exit normally or did not return 0. This is treated as an error by POP-C++.");
            mgr->KillAll();
          }
          LOG_DEBUG("Finalize stop");
          mgr->Stop(challenge);
          delete mgr;
        } catch(paroc_exception &e) {
            LOG_ERROR("while finalizing the application: %s", e.what());
        }
        mgr = NULL;
      }*/
#endif
}



/*AppCoreService *pop_system::CreateAppCoreService(char *codelocation)
{
    // Create challenge string
    srand(time(NULL));
    char tmp[256];

    for(int i=0; i<255; i++) {
        tmp[i]=(char)(1+254.0*rand()/(RAND_MAX+1.0) );
    }
    tmp[255]='\0';
    challenge=tmp;

    return new AppCoreService(challenge, false, codelocation);
}*/


void pop_system::processor_set(int /*cpu*/) {
#ifndef __APPLE__
    // Use glibc to set cpu affinity
    /*if (cpu < 0) {
        LOG_WARNING("Cannot set processor to %d<0", cpu);
        exit(EXIT_FAILURE);
    }
    if(cpu >= CPU_SETSIZE) {
        LOG_WARNING("Cannot set processor to %d while CPU_SETSIZE=%d", cpu, CPU_SETSIZE);
        exit(EXIT_FAILURE);
    }

    cpu_set_t cpu_set;
    CPU_ZERO(&cpu_set);
    CPU_SET(cpu, &cpu_set);
    if(sched_setaffinity(0, sizeof(cpu_set), &cpu_set) == -1) {
        LOG_WARNING("Cannot set processor to %d (cpu_set %p)", cpu,(void *)&cpu_set);
        exit(EXIT_FAILURE);
    }

    cpu_set_t cpu_get;
    CPU_ZERO(&cpu_get);
    if(sched_getaffinity(0, sizeof(cpu_get), &cpu_get) == -1) {
        LOG_WARNING("Unable to sched_getaffinity to (cpu_get) %p", (void *)&cpu_get);
        exit(EXIT_FAILURE);
    }

    if(memcmp(&cpu_get, &cpu_set, sizeof(cpu_set_t))) {
        LOG_WARNING("Unable to run on cpu %d", cpu);
        exit(EXIT_FAILURE);
    }
    #else
    // Apple thread API
    */

#endif
}
