#ifndef __QUICKDRAW_H__
#define __QUICKDRAW_H__

#include "mac.h"

struct mac_backend_userdata_t {
};

void	    carbonBackendUserDataInit(mac_backend_userdata);
mac_backend getCarbonBackend(void);

#endif
