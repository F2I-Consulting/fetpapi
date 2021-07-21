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

#include "DataArrayHandlers.h"

namespace ETP_NS
{
	template<class T>
	class FETPAPI_DLL_IMPORT_OR_EXPORT BlockingDataArrayHandlers : public DataArrayHandlers
	{
	public:
		BlockingDataArrayHandlers(AbstractSession* mySession, T* values): DataArrayHandlers(mySession), values(values) {}
		virtual ~BlockingDataArrayHandlers() = default;

		void on_GetDataArraysResponse(Energistics::Etp::v12::Protocol::DataArray::GetDataArraysResponse & gdar, int64_t correlationId) final;

	private:
		/** 
		*	The pointer must have been allocated with sufficient size and won't be deallocate by these protocol handlers.
		*/
		T* values;
	};

	template<class T> void BlockingDataArrayHandlers<T>::on_GetDataArraysResponse(Energistics::Etp::v12::Protocol::DataArray::GetDataArraysResponse & gdar, int64_t) {
		if (gdar.dataArrays.size() == 1) {
			auto dataArray = gdar.dataArrays.begin()->second;
			if (dataArray.data.item.idx() == 0) {
				Energistics::Etp::v12::Datatypes::ArrayOfBoolean& avroArray = dataArray.data.item.get_ArrayOfBoolean();
				for (auto i = 0; i < avroArray.values.size(); ++i) {
					values[i] = avroArray.values[i];
				}
			}
			else if (dataArray.data.item.idx() == 1) {
				Energistics::Etp::v12::Datatypes::ArrayOfInt& avroArray = dataArray.data.item.get_ArrayOfInt();
				for (auto i = 0; i < avroArray.values.size(); ++i) {
					values[i] = avroArray.values[i];
				}
			}
			else if (dataArray.data.item.idx() == 2) {
				Energistics::Etp::v12::Datatypes::ArrayOfLong& avroArray = dataArray.data.item.get_ArrayOfLong();
				for (auto i = 0; i < avroArray.values.size(); ++i) {
					values[i] = avroArray.values[i];
				}
			}
			else if (dataArray.data.item.idx() == 3) {
				Energistics::Etp::v12::Datatypes::ArrayOfFloat& avroArray = dataArray.data.item.get_ArrayOfFloat();
				for (auto i = 0; i < avroArray.values.size(); ++i) {
					values[i] = avroArray.values[i];
				}
			}
			else if (dataArray.data.item.idx() == 4) {
				Energistics::Etp::v12::Datatypes::ArrayOfDouble& avroArray = dataArray.data.item.get_ArrayOfDouble();
				for (auto i = 0; i < avroArray.values.size(); ++i) {
					values[i] = avroArray.values[i];
				}
			}
			/*
			else if (dataArray.data.item.idx() == 5) {
				Energistics::Etp::v12::Datatypes::ArrayOfString& avroArray = dataArray.data.item.get_ArrayOfString();
				for (auto i = 0; i < avroArray.values.size(); ++i) {
					values[i] = avroArray.values[i];
				}
			}
			*/
			else if (dataArray.data.item.idx() == 6) {
				std::string& avroValues = dataArray.data.item.get_bytes();
				for (auto i = 0; i < avroValues.size(); ++i) {
					values[i] = avroValues[i];
				}
			}
		}
	}
}
