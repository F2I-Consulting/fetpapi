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
#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#endif

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_generators.hpp>

#include <fesapi/common/HdfProxyFactory.h>
#include <fesapi/resqml2/PointSetRepresentation.h>
#include <fesapi/resqml2_0_1/LocalDepth3dCrs.h>

#include "catch.hpp"

#include "etp/EtpHelpers.h"
#include "etp/ClientSessionLaunchers.h"
#include "etp/fesapi/FesapiHdfProxy.h"
#include "etp/fesapi/FesapiHelpers.h"

TEST_CASE("Validate ETP URI", "[EtpUri]")
{
	REQUIRE(ETP_NS::EtpHelpers::validateUri("eml:///dataspace('test/test')"));
	REQUIRE_FALSE(ETP_NS::EtpHelpers::validateUri("eml://dataspace('test/test')")); // Missing slash
	REQUIRE(ETP_NS::EtpHelpers::validateUri("eml:///dataspace('test/test')/eml20.obj_EpcExternalPartReference(da9e0cc3-0f71-4fd9-83ad-a6334b9b0832)"));
	REQUIRE(ETP_NS::EtpHelpers::validateDataObjectUri("eml:///dataspace('test/test')/eml20.obj_EpcExternalPartReference(da9e0cc3-0f71-4fd9-83ad-a6334b9b0832)"));
	REQUIRE_FALSE(ETP_NS::EtpHelpers::validateDataObjectUri("eml:///dataspace('test/test')"));

	REQUIRE(ETP_NS::EtpHelpers::getDataspaceUri("eml:///dataspace('test/test')/eml20.obj_EpcExternalPartReference(da9e0cc3-0f71-4fd9-83ad-a6334b9b0832)") == "eml:///dataspace('test/test')");
	REQUIRE(ETP_NS::EtpHelpers::getDataObjectType("eml:///dataspace('test/test')/eml20.obj_EpcExternalPartReference(da9e0cc3-0f71-4fd9-83ad-a6334b9b0832)") == "eml20.obj_EpcExternalPartReference");
	REQUIRE(ETP_NS::EtpHelpers::getUuidAndVersionFromUri("eml:///dataspace('test/test')/eml20.obj_EpcExternalPartReference(da9e0cc3-0f71-4fd9-83ad-a6334b9b0832)").first == "da9e0cc3-0f71-4fd9-83ad-a6334b9b0832");
}

boost::uuids::random_generator gen;

std::shared_ptr<ETP_NS::AbstractSession> connect()
{
	//ETP_NS::InitializationParameters initializationParams(gen(), "ws://etp.f2i-consulting.com:9002/");
	ETP_NS::InitializationParameters initializationParams(gen(), "ws://127.0.0.1:9002/");
	std::map< std::string, std::string > additionalHeaderField = { {"data-partition-id", ""} };
	auto clientSession = ETP_NS::ClientSessionLaunchers::createClientSession(&initializationParams, "Basic Zm9vOmJhcg==");
	clientSession->setVerbose(false);

	std::thread sessionThread(&ETP_NS::ClientSession::run, clientSession);
	sessionThread.detach();

	// Wait for the ETP session to be opened
	auto t_start = std::chrono::high_resolution_clock::now();
	while (clientSession->isEtpSessionClosed()) {
		auto timeOut = std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - t_start).count();
		if (timeOut > 5000) {
			throw std::invalid_argument("Time out : " + std::to_string(timeOut) + " ms.\n");
		}
	}

	return clientSession;
}

std::string putDataspace(std::shared_ptr<ETP_NS::AbstractSession> session)
{
	std::string dataspaceUuid = boost::lexical_cast<std::string>(gen());
	Energistics::Etp::v12::Datatypes::Object::Dataspace dataspace;
	dataspace.uri = "eml:///dataspace('testF2I/" + dataspaceUuid + "')";
	dataspace.path = "testF2I/" + dataspaceUuid;
	std::map<std::string, Energistics::Etp::v12::Datatypes::Object::Dataspace> query = { {"0", dataspace} };
	std::vector<std::string> successKeys = session->putDataspaces(query);

	return successKeys.size() == 1 && successKeys[0] == "0" ? dataspace.uri : "";
}

void deleteDataspace(std::shared_ptr<ETP_NS::AbstractSession> session, const std::string & dataspaceUri)
{
	std::map< std::string, std::string > query = { { "0", dataspaceUri } };
	session->deleteDataspaces(query);
}

TEST_CASE("Put a DataArray", "[DataArray]")
{
	std::shared_ptr<ETP_NS::AbstractSession> session = connect();
	session->setVerbose(true);
	const std::string dataspaceUri = putDataspace(session);
	REQUIRE(dataspaceUri.size() > 0);

	// Initialize the FESAPI Repository
	COMMON_NS::DataObjectRepository repo;
	repo.setDefaultStandard(COMMON_NS::DataObjectRepository::EnergisticsStandard::RESQML2_0_1);
	repo.setDefaultStandard(COMMON_NS::DataObjectRepository::EnergisticsStandard::EML2_0);
	repo.setHdfProxyFactory(new ETP_NS::FesapiHdfProxyFactory(session.get()));

	// Create the point set representation, an ETP HDF proxy if necessary and a partial crs
	RESQML2_NS::PointSetRepresentation* h1i1PointSetRep = repo.createPointSetRepresentation("d95dcb6c-96df-4749-a481-5981390067f4", "Horizon1 Interp1 PointSetRep");
	h1i1PointSetRep->setUriSource(dataspaceUri);
	auto* crs = repo.createLocalDepth3dCrs("", "Testing local depth Crs",
		.0, .0, .0, .0, gsoap_resqml2_0_1::eml20__LengthUom::m, "Only for testing purpose", gsoap_resqml2_0_1::eml20__LengthUom::m, "Only for testing purpose", false);
	auto* etpHdfProxy = repo.createHdfProxy("f31fcc59-b8c4-4fc6-a524-973569b4cca5", "", "", "", COMMON_NS::DataObjectRepository::openingMode::READ_WRITE);
	etpHdfProxy->setUriSource(dataspaceUri);
	repo.setDefaultHdfProxy(etpHdfProxy);

	// Create and push the numerical values to the store
	// Internally it uses the ETP Hdf proxy set as the default HDF proxy of the repository in main.cpp.
	// pushBackXyzGeometryPatch is a blocking method. If you want non blocking method, you need to use PutDataArray directly.
	const size_t xyzPointCount = 1000000;
	std::unique_ptr<double[]> xyzPoints(new double[xyzPointCount * 3]);
	for (size_t ptIdx = 0; ptIdx < xyzPointCount; ptIdx++) {
		xyzPoints[ptIdx * 3] = (double)ptIdx;
		xyzPoints[ptIdx * 3 + 1] = (double)ptIdx;
		xyzPoints[ptIdx * 3 + 2] = (double)ptIdx;
	}
	std::cout << "size of the array : " << xyzPointCount * 3 * 8 << " bytes." << std::endl;
	auto t_start = std::chrono::high_resolution_clock::now();
	std::vector<std::string> dataspaceUris;
	dataspaceUris.push_back(dataspaceUri);
	std::string transactionFailure = session->startTransaction(dataspaceUris, false);
	REQUIRE(transactionFailure.empty());
	h1i1PointSetRep->pushBackXyzGeometryPatch(xyzPointCount, xyzPoints.get(), nullptr, crs);
	std::cout << "Put DataArray in : " << std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - t_start).count() << " milliseconds." << std::endl;
	
	// send the XML part
	repo.setUriSource(dataspaceUri);
	std::map<std::string, Energistics::Etp::v12::Datatypes::Object::DataObject> dataObjects;
	int index = 0;
	for (auto uuid : repo.getUuids()) {
		dataObjects[std::to_string(index++)] = ETP_NS::FesapiHelpers::buildEtpDataObjectFromEnergisticsObject(repo, uuid);
	}
	session->putDataObjects(dataObjects);
	transactionFailure = session->commitTransaction();
	REQUIRE(transactionFailure.empty());

	//Reading back
	std::unique_ptr<double[]> receivedXyzPoints(new double[xyzPointCount * 3]);
	t_start = std::chrono::high_resolution_clock::now();
	h1i1PointSetRep->getXyzPointsOfPatch(0, receivedXyzPoints.get());
	std::cout << "Get DataArray in : " << std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - t_start).count() << " milliseconds." << std::endl;

	// Cleaning
	deleteDataspace(session, dataspaceUri);
	session->close();

	for (size_t xyzPointIndex = 0; xyzPointIndex < xyzPointCount; ++xyzPointIndex) {
		REQUIRE(receivedXyzPoints[xyzPointIndex * 3] == xyzPointIndex);
		REQUIRE(receivedXyzPoints[xyzPointIndex * 3 + 1] == xyzPointIndex);
		REQUIRE(receivedXyzPoints[xyzPointIndex * 3 + 2] == xyzPointIndex);
	}
}
