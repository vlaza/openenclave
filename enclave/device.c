#include "device.h"

static size_t device_table_len = 0;
static oe_device_t *device_table; // Resizable array of device entries

size_t file_descriptor_table_len = 0;
oe_device_t *oe_file_descriptor_table 

int HostFSTableIdx   = -1;
int EnclaveLocalFSTableIdx = -1;
int HostNetworkTableIdx = -1;
int EnclaveToEnclaveNetworkTableIdx = -1;

static size_t resolver_table_len = 0;
static oe_resolver_t *resolver_table; // Resizable array of device entries

int _DefaultInitDevices()

{

    // Opt into file systems

    device_table = CreateHostFSDevice(device_table, &device_table_len, &HostFSTableIdx);
    device_table = CreateEnclaveLocalFSDevice(device_table, &device_table_len, &EnclaveLocalFSTableIdx);

    int rslt = (*device_table[HostFSTableIdx].Mount)("/host", READ_ONLY );
    int rslt = (*device_table[HostFSTableIdx].Mount)("/secure", READ_WRITE );

    // Opt into the network

    device_table = CreateHostNetwork(device_table, &device_table_len, &HostNetworkTableIdx);
    device_table = CreateEnclaveToEnclaveNetwork(device_table, &device_table_len, &EnclaveLocalNetworkTableIdx);

    // Opt into resolvers

    resolver_table = CreateHostResolver(resolver_table, &resolver_table_len, &HostResolverTableIdx);
    resolver_table = CreateEnclaveLocalDNSResolver(resolver_table, &resolver_table_len, &EnclaveLocalDNSResolverTableIdx);
    resolver_table = CreateEnclaveLocalResolver(resolver_table, &resolver_table_len, &EnclaveLocalResolverTableIdx);

}




