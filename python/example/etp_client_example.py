"""
Please first install the built wheel located in fetpapi/python/dist
"""

import sys
from threading import Thread
from time import sleep, perf_counter
import uuid
import fesapi
import fetpapi

def start_etp_server(client_session):
    print('Running client session...')
    client_session.run()
    print('Client session do no more run...')

etp_server_url = "wss://..."
authorization = "Bearer ..."
initialization_params = fetpapi.InitializationParameters(str(uuid.uuid4()), etp_server_url)
# OSDU RDDMS requires some additional header fields compared to ETP server
additionalHeaderField = fetpapi.MapStringString()
additionalHeaderField["data-partition-id"] = "osdu"
initialization_params.setAdditionalHandshakeHeaderFields(additionalHeaderField)

client_session = fetpapi.createClientSession(initialization_params, authorization)
client_session.setCoreProtocolHandlers(fetpapi.CoreHandlers(client_session));
client_session.setDataspaceProtocolHandlers(fetpapi.DataspaceHandlers(client_session));
client_session.setDiscoveryProtocolHandlers(fetpapi.DiscoveryHandlers(client_session));
client_session.setStoreProtocolHandlers(fetpapi.StoreHandlers(client_session));
client_session.setDataArrayProtocolHandlers(fetpapi.DataArrayHandlers(client_session));
t = Thread(target=start_etp_server, args=(client_session,), daemon=True)
t.start()
start_time = perf_counter()
while client_session.isEtpSessionClosed() and perf_counter() - start_time < 5:
    sleep(0.25)	
if client_session.isEtpSessionClosed():
    print("The ETP session could not be established in 5 seconds.")
    sys.exit()
print("Now connected to ETP Server")

# Find an available ETP dataspace
all_dataspaces = client_session.getDataspaces()
if all_dataspaces.empty() :
    print("There is no dataspace on this ETP server.")
    sys.exit()
for dataspace in all_dataspaces:
    print(dataspace.uri)
dataspace = all_dataspaces.front()

print("Working on dataspace " + dataspace.uri);
# List resources of this ETP dataspace
etp_context = fetpapi.ContextInfo()
etp_context.uri = dataspace.uri
etp_context.depth = 1
all_resources = client_session.getResources(etp_context, fetpapi.ContextScopeKind__self);
if all_resources.empty() :
    print("There is no resource on this dataspace.")
    sys.exit()
for resource in all_resources:
    print(resource.uri)

# Create a FESAPI Dataobject repository to ease access to dataobjects
repo = fesapi.DataObjectRepository()
hdf_proxy_factory = fetpapi.FesapiHdfProxyFactory(client_session)
repo.setHdfProxyFactory(hdf_proxy_factory)

# Get dataobjects from the resources to the DataObjectRepository
uriMap = fetpapi.MapStringString();
for index, resource in enumerate(all_resources):
    uriMap[str(index)] = resource.uri
all_resources = client_session.getDataObjects(uriMap);
for dataObject in all_resources.values():
    repo.addOrReplaceGsoapProxy(dataObject.data, fetpapi.getDataObjectType(dataObject.resource.uri), fetpapi.getDataspaceUri(dataObject.resource.uri))

# Read data of the first ijk grid
if repo.getIjkGridRepresentationCount() > 0:
    ijk_grid = repo.getIjkGridRepresentation(0)
    print("IJK Grid : " + ijk_grid.getTitle())
    print(f"iCellCount : {ijk_grid.getICellCount()} jCellCount : {ijk_grid.getJCellCount()} kCellCount : {ijk_grid.getKCellCount()}")
    nb_xyz_points = ijk_grid.getXyzPointCountOfAllPatches()
    print("XYZ points count :", nb_xyz_points)
    xyz_points = fesapi.DoubleArray(nb_xyz_points * 3)
    ijk_grid.getXyzPointsOfAllPatches(xyz_points)

    ijk_grid.loadSplitInformation()
    origin_index = ijk_grid.getXyzPointIndexFromCellCorner(0, 0, 0, 0)
    print("Cell 0,0,0 corner 0 is at index ", origin_index)
    print("Cell 0,0,0 corner 0 is  ", xyz_points.getitem(origin_index * 3), " ", xyz_points.getitem(origin_index * 3 + 1), " ", xyz_points.getitem(origin_index * 3 + 2))
    ijk_grid.unloadSplitInformation()
else:
    print("This dataspace has no IJK Grid")

# Read data of the first 2d grid
if repo.getHorizonGrid2dRepresentationCount() > 0:
    grid2d = repo.getHorizonGrid2dRepresentation(0)
    print("2d Grid : " + grid2d.getTitle())
    print(f"iCount : {grid2d.getNodeCountAlongIAxis()} jCount : {grid2d.getNodeCountAlongJAxis()}")
    nb_z_points = grid2d.getNodeCountAlongIAxis() * grid2d.getNodeCountAlongJAxis()
    print(f"XYZ points count : {nb_z_points}")
    z_points = fesapi.DoubleArray(nb_z_points)
    grid2d.getZValues(z_points)

    print("Z value at index 0 : ", z_points.getitem(0))
    print("Z value at index 1 : ", z_points.getitem(1))
else:
    print("This dataspace has no 2d Grid")

client_session.close();
print("FINISHED")
