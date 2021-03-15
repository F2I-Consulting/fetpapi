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
#pragma once

#include "../../nsDefinitions.h"
#include "../EtpMessages.h"

#if defined(_WIN32) && !defined(FETPAPI_STATIC)
	#ifndef FETPAPI_DLL_IMPORT_OR_EXPORT
		#if defined(Fetpapi_EXPORTS)
			#define FETPAPI_DLL_IMPORT_OR_EXPORT __declspec(dllexport)
		#else
			#define FETPAPI_DLL_IMPORT_OR_EXPORT __declspec(dllimport)
		#endif
	#endif
#else
	#define FETPAPI_DLL_IMPORT_OR_EXPORT
#endif

namespace COMMON_NS
{
	class AbstractObject;
}

namespace ETP_NS
{
	class AbstractSession;

	namespace FesapiHelpers {
		/**
		* Build and return an ETP resource from an Energistics object.
		* @param obj		The input Energistics obj
		* @param countRels	Indicate if the returned resource contain the count of source or target relationships.
		* @return			The ETP resource built from the Energistics object
		*/
		FETPAPI_DLL_IMPORT_OR_EXPORT Energistics::Etp::v12::Datatypes::Object::Resource buildEtpResourceFromEnergisticsObject(const COMMON_NS::AbstractObject * const obj, bool countRels = true);

		/**
		* Build and return an ETP data object from an Energistics object.
		* @param obj	The input Energistics obj
		* @return		The ETP data object built from the Energistics object
		*/
		FETPAPI_DLL_IMPORT_OR_EXPORT Energistics::Etp::v12::Datatypes::Object::DataObject buildEtpDataObjectFromEnergisticsObject(COMMON_NS::AbstractObject * obj, bool includeSerialization = true);
	}
}