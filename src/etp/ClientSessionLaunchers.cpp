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
#include "ClientSessionLaunchers.h"

#include <sstream>

#include "HttpClientSession.h"
#include "PlainClientSession.h"

#if WITH_ETP_SSL
#include "ssl/HttpsClientSession.h"
#include "ssl/SslClientSession.h"
#endif

namespace
{
	std::size_t getNegotiatedMaxWebSocketFramePayloadSize(const std::string& responseBody, std::size_t preferredMaxFrameSize) {
		const auto maxWebSocketFramePayloadSizePos = responseBody.find("MaxWebSocketFramePayloadSize");
		if (maxWebSocketFramePayloadSizePos == std::string::npos) return preferredMaxFrameSize;

		std::istringstream iss(responseBody);
		iss.seekg(maxWebSocketFramePayloadSizePos);

		std::string temp;
		std::size_t serverMaxWebSocketFramePayloadSize;
		while (!iss.eof()) {
			/* extracting word by word from stream */
			iss >> temp;

			/* Checking the given word is integer or not */
			if (std::istringstream(temp) >> serverMaxWebSocketFramePayloadSize) {
				return std::min(serverMaxWebSocketFramePayloadSize, preferredMaxFrameSize);
			}
		}
		return preferredMaxFrameSize;
	}
}

std::shared_ptr<ETP_NS::ClientSession> ETP_NS::ClientSessionLaunchers::createClientSession(InitializationParameters const* initializationParams,
	const std::string& authorization, const std::string& proxyAuthorization)
{
	boost::asio::io_context ioc;

	const std::string etpServerUrlPath = initializationParams->getEtpServerUrlPath();
	const std::string etpServerCapTarget = "/" + etpServerUrlPath +
		(!etpServerUrlPath.empty() && etpServerUrlPath.back() != '/'
			? "/.well-known/etp-server-capabilities?GetVersion=etp12.energistics.org"
			: ".well-known/etp-server-capabilities?GetVersion=etp12.energistics.org");

	std::shared_ptr<ETP_NS::ClientSession> result;
#if WITH_ETP_SSL
	if (initializationParams->getEtpServerPort() == 443 || initializationParams->isTlsForced()) {
		// The SSL context is required, and holds certificates
		// From official ETP documentation : If the ETP server is supporting TLS, it MUST support v1.2 or greater
#if USE_WINTLS_INSTEAD_OF_OPENSSL
		wintls::context ctx{ wintls::method::tlsv12_client };
		// Use the operating systems default certificates for verification
		ctx.use_default_certificates(true);
		// Verify the remote server's certificate
		ctx.verify_server_certificate(true);
#else
		boost::asio::ssl::context ctx{ boost::asio::ssl::context::tlsv12_client };
		// Use the operating systems default certificates for verification
		boost::system::error_code ec;
		ctx.set_default_verify_paths(ec);
		if (ec) {
			std::cerr << "Default verify paths failed: " << ec.message() << std::endl;
			// Fallback: provide explicit CA bundle
			// ctx.load_verify_file("cacert.pem");
		}
		// Verify the remote server's certificate
		ctx.set_verify_mode(ssl::context::verify_peer);
		ctx.set_options(
			boost::asio::ssl::context::default_workarounds
			| boost::asio::ssl::context::no_sslv2
			| boost::asio::ssl::context::no_sslv3
			| boost::asio::ssl::context::no_tlsv1
			| boost::asio::ssl::context::no_tlsv1_1
			| boost::asio::ssl::context::single_dh_use
		);
#endif

		const std::string& additionalCertificates = initializationParams->getAdditionalCertificates();
		if (!additionalCertificates.empty()) {
#if USE_WINTLS_INSTEAD_OF_OPENSSL
			wintls::error_code ec;
			std::cerr << "Cannot add certificates using wintls for now" << std::endl;
#else
			ctx.add_certificate_authority(
				boost::asio::buffer(additionalCertificates.data(), additionalCertificates.size()), ec);
#endif
			if (ec) {
				std::cerr << "Cannot add certificates : " << additionalCertificates << std::endl;
			}
		}
		auto restClientSession = std::make_shared<HttpsClientSession>(ioc, ctx);
		restClientSession->run(
			initializationParams->getEtpServerHost(), initializationParams->getEtpServerPort(), etpServerCapTarget, 11, authorization,
			initializationParams->getProxyHost(), initializationParams->getProxyPort(), proxyAuthorization);
		// Run the I/O service. The call will return when the get operation is complete.
		ioc.run();

		std::size_t preferredMaxFrameSize = getNegotiatedMaxWebSocketFramePayloadSize(restClientSession->getResponse().body(), initializationParams->getPreferredMaxFrameSize());

		result = std::make_shared<SslClientSession>(std::move(ctx), initializationParams, "/" + initializationParams->getEtpServerUrlPath(),
			authorization, proxyAuthorization,
			initializationParams->getAdditionalHandshakeHeaderFields(), preferredMaxFrameSize);
	}
	else {
#endif
		auto restClientSession = std::make_shared<HttpClientSession>(ioc);
		restClientSession->run(
			initializationParams->getEtpServerHost(), initializationParams->getEtpServerPort(), etpServerCapTarget, 11, authorization,
			initializationParams->getProxyHost(), initializationParams->getProxyPort(), proxyAuthorization);
		// Run the I/O service. The call will return when the get operation is complete.
		ioc.run();

		std::size_t preferredMaxFrameSize = getNegotiatedMaxWebSocketFramePayloadSize(restClientSession->getResponse().body(), initializationParams->getPreferredMaxFrameSize());

		result = std::make_shared<PlainClientSession>(initializationParams, "/" + initializationParams->getEtpServerUrlPath(),
			authorization, proxyAuthorization,
			initializationParams->getAdditionalHandshakeHeaderFields(), preferredMaxFrameSize);
#if WITH_ETP_SSL
	}
#endif

	initializationParams->postSessionCreationOperation(result.get());
	return result;
}
