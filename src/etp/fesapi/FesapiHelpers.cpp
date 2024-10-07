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
#include "FesapiHelpers.h"

Energistics::Etp::v12::Datatypes::Object::Resource ETP_NS::FesapiHelpers::buildEtpResourceFromEnergisticsObject(COMMON_NS::AbstractObject const* obj, bool countRels)
{
	if (obj == nullptr) {
		throw std::invalid_argument("Cannot build resource from a null object.");
	}

	Energistics::Etp::v12::Datatypes::Object::Resource result;

	result.uri = obj->buildEtp12Uri();
	result.name = obj->getTitle();
	if (obj->isPartial()) {
		result.lastChanged = -1;
	}
	else {
		const time_t lastUpdate = obj->getLastUpdate();
		result.lastChanged = (lastUpdate > -1 ? lastUpdate : obj->getCreation()) * 1000000;
	}
	
	if (countRels) {
		result.sourceCount = obj->getRepository()->getSourceObjects(obj).size();
		result.targetCount = obj->getRepository()->getTargetObjects(obj).size();
	}

	result.storeLastWrite = -1; // Not supported yet

	result.activeStatus = Energistics::Etp::v12::Datatypes::Object::ActiveStatusKind::Inactive; // Active dataobjects are not supported for now

	return result;
}

Energistics::Etp::v12::Datatypes::Object::Resource ETP_NS::FesapiHelpers::buildEtpResourceFromEnergisticsObject(
	COMMON_NS::DataObjectRepository const* repo, const std::string& uuid, bool countRels) {
	return buildEtpResourceFromEnergisticsObject(repo->getDataObjectByUuid(uuid), countRels);
}

Energistics::Etp::v12::Datatypes::Object::DataObject ETP_NS::FesapiHelpers::buildEtpDataObjectFromEnergisticsObject(COMMON_NS::AbstractObject * obj, bool includeSerialization)
{
	Energistics::Etp::v12::Datatypes::Object::DataObject result;
	if (includeSerialization) {
		if (obj == nullptr) {
			throw std::invalid_argument("Cannot build an ETP dataobject from a null object.");
		}
		if (obj->isPartial()) {
			obj = obj->getRepository()->resolvePartial(obj);
			if (obj == nullptr) {
				throw std::invalid_argument("Cannot build an ETP dataobject from a partial object which is not resolvable");
			}
		}
		result.format = "xml";
		result.data = obj->serializeIntoString();
	}
	result.resource = ETP_NS::FesapiHelpers::buildEtpResourceFromEnergisticsObject(obj);

	return result;
}

Energistics::Etp::v12::Datatypes::Object::DataObject ETP_NS::FesapiHelpers::buildEtpDataObjectFromEnergisticsObject(
	COMMON_NS::DataObjectRepository const* repo, const std::string& uuid, bool includeSerialization) {
	return buildEtpDataObjectFromEnergisticsObject(repo->getDataObjectByUuid(uuid), includeSerialization);
}
