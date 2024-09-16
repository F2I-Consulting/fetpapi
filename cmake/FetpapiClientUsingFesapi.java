/*-----------------------------------------------------------------------
Licensed to the Apache Software Foundation (ASF) under one
or more contributor license agreements.  See the NOTICE file
distributed with this work for additional information
regarding copyright ownership.  The ASF licenses this file
to you under the Apache License, Version 2.0 (the
"License"; you may not use this file except in compliance
with the License.  You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing,
software distributed under the License is distributed on an
"AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
KIND, either express or implied.  See the License for the
specific language governing permissions and limitations
under the License.
-----------------------------------------------------------------------*/
package com.f2i_consulting.fetpapi.client;

import java.util.Optional;
import java.util.UUID;
import java.util.concurrent.TimeUnit;

import com.f2i_consulting.fesapi.SWIGTYPE_p_double;
import com.f2i_consulting.fesapi.fesapi;
import com.f2i_consulting.fesapi.common.DataObjectRepository;
import com.f2i_consulting.fesapi.resqml2.AbstractValuesProperty;
import com.f2i_consulting.fesapi.resqml2.IjkGridExplicitRepresentation;
import com.f2i_consulting.fetpapi.DataspaceVector;
import com.f2i_consulting.fetpapi.MapStringDataObject;
import com.f2i_consulting.fetpapi.MapStringString;
import com.f2i_consulting.fetpapi.ResourceVector;
import com.f2i_consulting.fetpapi.fetpapi;
import com.f2i_consulting.fetpapi.Energistics.Etp.v12.Datatypes.Object.ContextInfo;
import com.f2i_consulting.fetpapi.Energistics.Etp.v12.Datatypes.Object.ContextScopeKind;
import com.f2i_consulting.fetpapi.Energistics.Etp.v12.Datatypes.Object.DataObject;
import com.f2i_consulting.fetpapi.Energistics.Etp.v12.Datatypes.Object.Dataspace;
import com.f2i_consulting.fetpapi.Energistics.Etp.v12.Datatypes.Object.Resource;
import com.f2i_consulting.fetpapi.etp.ClientSession;
import com.f2i_consulting.fetpapi.etp.CoreHandlers;
import com.f2i_consulting.fetpapi.etp.DataArrayHandlers;
import com.f2i_consulting.fetpapi.etp.DataspaceHandlers;
import com.f2i_consulting.fetpapi.etp.DiscoveryHandlers;
import com.f2i_consulting.fetpapi.etp.FesapiHdfProxyFactory;
import com.f2i_consulting.fetpapi.etp.InitializationParameters;
import com.f2i_consulting.fetpapi.etp.StoreHandlers;

public class FetpapiClientUsingFesapi {

	/**
	 * Loading the FesapiCpp and Fetpapi native libraries
	 */
	static {
		try {
			System.loadLibrary("${FESAPI_ASSEMBLY_NAME}");
		}
		catch (UnsatisfiedLinkError e) {
			System.out.println("UnsatisfiedLinkError : " + e.toString());
		}
		try {
			System.loadLibrary("${ASSEMBLY_NAME}");
		}
		catch (UnsatisfiedLinkError e) {
			System.out.println("UnsatisfiedLinkError : " + e.toString());
		}
	}

	public static void main(String[] args) {
		// Read arguments of the command line
		String serverUrl = "ws://etp.f2i-consulting.com:9002/";
		String authorization = "";
		
		if (args.length < 1) {
			System.out.println("Program arguments should be : serverUrl \"<BearerOrBasic> <token>\"");
			System.out.println("WARNING : you did not provide a server URL. The URL will be defaulted to ws://etp.f2i-consulting.com:9002/");
		}
		else {
			serverUrl = args[0];
			if (args.length < 2) {
				System.out.println("Program arguments should be : serverUrl \"Bearer <token>\"");
				System.out.println("WARNING : You did not provide any authorizaton after the URL. Please see https://developer.mozilla.org/en-US/docs/Web/HTTP/Headers/Authorization for further details");
			}
			else {
				if (!args[1].startsWith("Basic ") && !args[1].startsWith("Bearer ")) {
					System.out.println("WARNING : HTTP authorization \"" + args[3] + "\" field does not start with \"Basic \" nor \"Bearer \" which is unlikely");
				}
				authorization = args[1];
			}
		}
		
		// Create a DataObjectRepository in order to store retrieved dataobjects from the ETP server
		try (DataObjectRepository repo = new DataObjectRepository()) {
			repo.setDefaultStandard(DataObjectRepository.EnergisticsStandard.RESQML2_0_1);
			repo.setDefaultStandard(DataObjectRepository.EnergisticsStandard.EML2_0);

			// ****** Connect to ETP server through clientSession ******
			String instanceUuid = UUID.randomUUID().toString(); 
			InitializationParameters initializationParams = new InitializationParameters(instanceUuid, serverUrl);
			MapStringString additionalHeaderField = new MapStringString();
			additionalHeaderField.put("data-partition-id", "osdu"); // Example for OSDU RDDMS
			initializationParams.setAdditionalHandshakeHeaderFields(additionalHeaderField);
			
			ClientSession clientSession = fetpapi.createClientSession(initializationParams, authorization);
			clientSession.setCoreProtocolHandlers(new CoreHandlers(clientSession));
			clientSession.setDataspaceProtocolHandlers(new DataspaceHandlers(clientSession));
			clientSession.setDiscoveryProtocolHandlers(new DiscoveryHandlers(clientSession));
			clientSession.setStoreProtocolHandlers(new StoreHandlers(clientSession));
			clientSession.setDataArrayProtocolHandlers(new DataArrayHandlers(clientSession));
			new Thread(clientSession::run).start();
			long start = System.currentTimeMillis();
			while (clientSession.isEtpSessionClosed() && System.currentTimeMillis() - start < 5000) {
				TimeUnit.MILLISECONDS.sleep(1);
			}			
			if (clientSession.isEtpSessionClosed()) {
				System.err.println("The ETP session cound not be establisehd in 5 seconds.");
				return;
			}
			System.out.println("Now connected to ETP Server");
			// ****** We are now connected to ETP server through clientSession ******
			// Set the HDF proxy factory in order to use one compliant with ETP
			repo.setHdfProxyFactory(new FesapiHdfProxyFactory(clientSession));
	
			// ****** Get a dataspace content. This corresponds to getting the content of an EPC file ******
			// Find an available ETP dataspace
			DataspaceVector allDataspaces = clientSession.getDataspaces();
			Optional<Dataspace> dataspace = allDataspaces.stream().findAny();		
			if (dataspace.isEmpty()) {
				clientSession.close();
				System.err.println("The ETP server has no dataspace.");
				return;
			}
			System.out.println("Working on dataspace " + dataspace.get().getUri());
			// List resources of this ETP dataspace
			ContextInfo etpContext = new ContextInfo();
			etpContext.setUri(dataspace.get().getUri());
			etpContext.setDepth(1);
			ResourceVector allResources = clientSession.getResources(etpContext, ContextScopeKind.self);
			if (allResources.isEmpty()) {
				clientSession.close();
				System.err.println("The ETP dataspace has no resource.");
				return;
			}
			// Get dataobjects from the resources to the DataObjectRepository
			MapStringString uriMap = new MapStringString();
			long index = 0;
			for (Resource resource : allResources) {
				uriMap.put(Long.toString(index++), resource.getUri());
			}
			MapStringDataObject allDataObjects = clientSession.getDataObjects(uriMap);
			for (DataObject dataObject : allDataObjects.values()) {
				repo.addOrReplaceGsoapProxy(dataObject.getData(), fetpapi.getDataObjectType(dataObject.getResource().getUri()), fetpapi.getDataspaceUri(dataObject.getResource().getUri()));
			}
			// ****** We have now in the DataObjectRepository the same content as if we would have deserialized and EPC file looking like the dataspace ******
			
			// ****** Use the DataObjectRepository exactly as you are used to do with FESAPI ******
			if (repo.getIjkGridRepresentationCount() > 0) {
				IjkGridExplicitRepresentation ijkGrid = repo.getIjkGridExplicitRepresentation(0);
				ijkGrid.loadSplitInformation();
				long originIndex = ijkGrid.getXyzPointIndexFromCellCorner(0, 0, 0, 0);
				System.out.println("The index of the grid origin in XYZ points is : " + originIndex);
				ijkGrid.unloadSplitInformation();
				if (ijkGrid.getValuesPropertyCount() > 0) {
					AbstractValuesProperty prop = ijkGrid.getValuesProperty(0);
					SWIGTYPE_p_double propValues = fesapi.new_DoubleArray(prop.getValuesCountOfPatch(0));
					try {
						prop.getDoubleValuesOfPatch(0, propValues);
						System.out.println("The first cell value of prop " + prop.getTitle() + " is " + fesapi.DoubleArray_getitem(propValues, 0));
					}
					finally {
						fesapi.delete_DoubleArray(propValues);
					}
				}
			}
			else {
				System.out.println("This dataspace has no IJK Grid");
			}
			
			// Do not forget to close session once you have processed all the dataobject repository.
			System.out.println("Closing the session...");
			clientSession.close();			
		} catch (InterruptedException e) {
			e.printStackTrace();
		}

		System.out.println("FINISHED");
	}
}
