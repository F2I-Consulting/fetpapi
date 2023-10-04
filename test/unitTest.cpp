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

#include <fesapi/common/HdfProxyFactory.h>
#include <fesapi/resqml2/PointSetRepresentation.h>
#include <fesapi/resqml2_0_1/LocalDepth3dCrs.h>

#include "catch.hpp"

#include "etp/ClientSessionLaunchers.h"
#include "etp/fesapi/FesapiHdfProxy.h"
#include "etp/fesapi/FesapiHelpers.h"

boost::uuids::random_generator gen;

std::shared_ptr<ETP_NS::AbstractSession> connect()
{
	std::shared_ptr<ETP_NS::AbstractSession> session = nullptr;

	ETP_NS::InitializationParameters initializationParams(gen(), ETP_SERVER_URL);
	std::map< std::string, std::string > additionalHeaderField = { {"data-partition-id", OSDU_PARTITION} };

#ifdef WITH_ETP_SSL
	if (std::string(ETP_SERVER_URL).find("wss://") == 0) {
		session = ETP_NS::ClientSessionLaunchers::createWssClientSession(&initializationParams, OSDU_AUTH, additionalHeaderField);
	}
	else {
#endif
		session = ETP_NS::ClientSessionLaunchers::createWsClientSession(&initializationParams, OSDU_AUTH);
#ifdef WITH_ETP_SSL
	}
#endif

	session->setStoreProtocolHandlers(std::make_shared<ETP_NS::StoreHandlers>(session.get()));
	session->setDataArrayProtocolHandlers(std::make_shared<ETP_NS::DataArrayHandlers>(session.get()));
	session->setDataspaceProtocolHandlers(std::make_shared<ETP_NS::DataspaceHandlers>(session.get()));

#ifdef WITH_ETP_SSL
	if (std::string(ETP_SERVER_URL).find("wss://") == 0) {
		auto sslSession = std::dynamic_pointer_cast<ETP_NS::SslClientSession>(session);
		std::thread sessionThread(&ETP_NS::SslClientSession::run, sslSession);
		sessionThread.detach();
	}
	else {
#endif
		auto plainSession = std::dynamic_pointer_cast<ETP_NS::PlainClientSession>(session);
		std::thread sessionThread(&ETP_NS::PlainClientSession::run, plainSession);
		sessionThread.detach();
#ifdef WITH_ETP_SSL
	}
#endif

	// Wait for the ETP session to be opened
	auto t_start = std::chrono::high_resolution_clock::now();
	while (session->isEtpSessionClosed()) {
		auto timeOut = std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - t_start).count();
		if (timeOut > 5000) {
			throw std::invalid_argument("Time out : " + std::to_string(timeOut) + " ms.\n");
		}
	}

	return session;
}

std::string putDataspace(std::shared_ptr<ETP_NS::AbstractSession> session)
{
	std::string dataspaceUuid = boost::lexical_cast<std::string>(gen());
	Energistics::Etp::v12::Datatypes::Object::Dataspace dataspace;
	dataspace.uri = "eml:///dataspace('testF2I/" + dataspaceUuid + "')";
	dataspace.path = "testF2I/" + dataspaceUuid;
	dataspace.storeCreated = 0;
	dataspace.storeLastWrite = 0;
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
	//session->setVerbose(true);
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
	auto* crs = repo.createPartial<RESQML2_0_1_NS::LocalDepth3dCrs>("", "");
	auto* etpHdfProxy = repo.createHdfProxy("f31fcc59-b8c4-4fc6-a524-973569b4cca5", "", "", "", COMMON_NS::DataObjectRepository::openingMode::READ_WRITE);
	etpHdfProxy->setUriSource(dataspaceUri);
	repo.setDefaultHdfProxy(etpHdfProxy);

	// Create and push the numerical values to the store
	// Internally it uses the ETP Hdf proxy set as the default HDF proxy of the repository in main.cpp.
	// pushBackGeometryPatch is a blocking method. If you want non blocking method, you need to use PutDataArray directly.
	const size_t xyzPointCount = 333000;
	std::unique_ptr<double[]> xyzPoints(new double[xyzPointCount * 3]);
	for (size_t ptIdx = 0; ptIdx < xyzPointCount; ptIdx++) {
		xyzPoints[ptIdx * 3] = ptIdx;
		xyzPoints[ptIdx * 3 + 1] = ptIdx;
		xyzPoints[ptIdx * 3 + 2] = ptIdx;
	}
	std::cout << "size of the array : " << xyzPointCount * 3 * 8 << " bytes." << std::endl;
	auto t_start = std::chrono::high_resolution_clock::now();
	h1i1PointSetRep->pushBackGeometryPatch(xyzPointCount, xyzPoints.get(), nullptr, crs);
	std::cout << "Put DataArray in : " << std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - t_start).count() << " milliseconds." << std::endl;
	
	// send the XML part
	// std::map<std::string, Energistics::Etp::v12::Datatypes::Object::DataObject> dataObjects = {
	// 	{"0", ETP_NS::FesapiHelpers::buildEtpDataObjectFromEnergisticsObject(h1i1PointSetRep)},
	// 	{"1", ETP_NS::FesapiHelpers::buildEtpDataObjectFromEnergisticsObject(etpHdfProxy)},
	// };
	// session->putDataObjects(dataObjects);
	
	//Reading back
	std::unique_ptr<double[]> receivedXyzPoints(new double[xyzPointCount * 3]);
	t_start = std::chrono::high_resolution_clock::now();
	h1i1PointSetRep->getXyzPointsOfPatch(0, receivedXyzPoints.get());
	std::cout << "Get DataArray in : " << std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - t_start).count() << " milliseconds." << std::endl;

	// Cleaning
	deleteDataspace(session, dataspaceUri);
	session->close();

	for (size_t xyzPointIndex = 0; xyzPointIndex < xyzPointCount && xyzPointIndex < 20; ++xyzPointIndex) {
		REQUIRE(receivedXyzPoints[xyzPointIndex * 3] == xyzPointIndex);
		REQUIRE(receivedXyzPoints[xyzPointIndex * 3 + 1] == xyzPointIndex);
		REQUIRE(receivedXyzPoints[xyzPointIndex * 3 + 2] == xyzPointIndex);
	}
	for (int64_t xyzPointIndex = xyzPointCount - 1; xyzPointIndex >= 0 && xyzPointIndex > xyzPointCount - 20; --xyzPointIndex) {
		REQUIRE(receivedXyzPoints[xyzPointIndex * 3] == xyzPointIndex);
		REQUIRE(receivedXyzPoints[xyzPointIndex * 3 + 1] == xyzPointIndex);
		REQUIRE(receivedXyzPoints[xyzPointIndex * 3 + 2] == xyzPointIndex);
	}

}
