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

etp_server_url = "ws://etp.f2i-consulting.com:9002/"
authorization = "Basic Zm9vOmJhcg=="
initialization_params = fetpapi.InitializationParameters(str(uuid.uuid4()), etp_server_url)
# OSDU RDDMS requires some additional header fields compared to ETP server
additionalHeaderField = fetpapi.MapStringString()
additionalHeaderField["data-partition-id"] = "osdu"
initialization_params.setAdditionalHandshakeHeaderFields(additionalHeaderField)

print("Creating the session...")
client_session = fetpapi.createClientSession(initialization_params, authorization)
t = Thread(target=start_etp_server, args=(client_session,), daemon=True)
print("Trying to connect to " + etp_server_url + " ...")
t.start()
start_time = perf_counter()
while client_session.isEtpSessionClosed() and perf_counter() - start_time < 5:
    sleep(0.25)
    print("Trying to connect to " + etp_server_url + " ...")
if client_session.isEtpSessionClosed():
    print("The ETP session could not be established in 5 seconds.")
    sys.exit()
print("Now connected to ETP Server")

print("List dataspaces")
all_dataspaces = client_session.getDataspaces()
if all_dataspaces.empty() :
    print("There is no dataspace on this ETP server.")
    sys.exit()
for dataspace in all_dataspaces:
    print(dataspace.uri)
dataspace = all_dataspaces.front()

print("List resources of dataspace " + dataspace.uri)
etp_context = fetpapi.ContextInfo()
etp_context.uri = dataspace.uri
etp_context.depth = 1
etp_context.navigableEdges = fetpapi.RelationshipKind_Both
etp_context.includeSecondaryTargets = False
etp_context.includeSecondarySources = False
all_resources = client_session.getResources(etp_context, fetpapi.ContextScopeKind__self)
if all_resources.empty() :
    print("There is no resource on this dataspace.")
    sys.exit()
else :
    print("There are " + str(len(all_resources)) + " resource on this dataspace.")
for index, resource in enumerate(all_resources):
    print(str(index) + ' : ' + resource.uri)

print("Create a FESAPI Dataobject repository to ease access to dataobjects");
repo = fesapi.DataObjectRepository()
hdf_proxy_factory = fetpapi.FesapiHdfProxyFactory(client_session)
print("Set specialized HdfProxy to deal with ETP DataArray subprotocol")
repo.setHdfProxyFactory(hdf_proxy_factory)

print("Get dataobjects from the resources to the DataObjectRepository");
uriMap = fetpapi.MapStringString();
for index, resource in enumerate(all_resources):
    uriMap[str(index)] = resource.uri
all_resources = client_session.getDataObjects(uriMap);
for dataObject in all_resources.values():
    repo.addOrReplaceGsoapProxy(dataObject.data, fetpapi.getDataObjectType(dataObject.resource.uri), fetpapi.getDataspaceUri(dataObject.resource.uri))

print("Read data of the first ijk grid");
if repo.getIjkGridRepresentationCount() > 0:
    ijk_grid = repo.getIjkGridRepresentation(0)
    print("IJK Grid : " + ijk_grid.getTitle())
    print(f"iCellCount : {ijk_grid.getICellCount()} jCellCount : {ijk_grid.getJCellCount()} kCellCount : {ijk_grid.getKCellCount()}")
    nb_xyz_points = ijk_grid.getXyzPointCountOfAllPatches()
    print("XYZ points count :", nb_xyz_points)
    xyz_points = fesapi.DoubleArray(nb_xyz_points * 3)
    ijk_grid.getXyzPointsOfAllPatches(xyz_points.cast())

    ijk_grid.loadSplitInformation()
    origin_index = ijk_grid.getXyzPointIndexFromCellCorner(0, 0, 0, 0)
    print("Cell 0,0,0 corner 0 is at index ", origin_index)
    print("Cell 0,0,0 corner 0 is  ", xyz_points.getitem(origin_index * 3), " ", xyz_points.getitem(origin_index * 3 + 1), " ", xyz_points.getitem(origin_index * 3 + 2))
    ijk_grid.unloadSplitInformation()

    if ijk_grid.getValuesPropertyCount() > 0:
        prop = ijk_grid.getValuesProperty(0)
        print("Prop at index 0 : " + prop.getTitle())
        print(type(prop))

        if isinstance(prop, fesapi.Resqml2_ContinuousProperty):
            prop_values = fesapi.DoubleArray(ijk_grid.getICellCount() * ijk_grid.getJCellCount() * ijk_grid.getKCellCount())
            prop.getDoubleValuesOfPatch(0, prop_values.cast())
            print("Cell 0,0,0 has prop value ", prop_values.getitem(0))
            print("Cell 1,0,0 has prop value ", prop_values.getitem(1))
            print("Cell 2,0,0 has prop value ", prop_values.getitem(2))
            print("Cell 3,0,0 has prop value ", prop_values.getitem(3))
            print("Cell 4,0,0 has prop value ", prop_values.getitem(4))
            print("Cell 5,0,0 has prop value ", prop_values.getitem(5))
        else:
            print(f"This property is a {type(prop)}")
    else:
        print("This IJK grid has no property")
else:
    print("This dataspace has no IJK Grid")

print("Read data of the first 2d grid");
if repo.getHorizonGrid2dRepresentationCount() > 0:
    grid2d = repo.getHorizonGrid2dRepresentation(0)
    print("2d grid : " + grid2d.getTitle())
    print(f"iCount : {grid2d.getNodeCountAlongIAxis()} jCount : {grid2d.getNodeCountAlongJAxis()}")
    nb_z_points = grid2d.getNodeCountAlongIAxis() * grid2d.getNodeCountAlongJAxis()
    print(f"XYZ points count : {nb_z_points}")
    z_points = fesapi.DoubleArray(nb_z_points)
    grid2d.getZValues(z_points.cast())

    print("Z value at index 0 : ", z_points.getitem(0))
    print("Z value at index 1 : ", z_points.getitem(1))

    if grid2d.getValuesPropertyCount() > 0:
        prop = grid2d.getValuesProperty(0)
        print("Prop at index 0 : " + prop.getTitle())
    else:
        print("This 2d grid has no property")
else:
    print("This dataspace has no 2d Grid")

repo.clear()
client_session.close()
print("FINISHED")
