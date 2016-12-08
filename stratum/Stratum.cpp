#include <string.h>
#include <stdlib.h>
#include <string>
#include "gason--/src/gason.hpp"
#include "Stratum.h"

namespace stratum
{

	void Stratum::Process(const char* str)
	{
		size_t len = strlen(str);
		if (len > 0)
		{
			char* nstr = (char*)malloc(len);
			memcpy(nstr, str, len);
			Process(nstr);
			free(nstr);
		}
	}

	enum StratumMethod
	{
		eMethodNone,
		eMethodMiningSubscribe,
		eMethodMiningAuthorize,
		eMethodMiningExtranonceSubscribe,
		eMethodMiningSetTarget,
		eMethodMiningSetExtranonce,
		eMethodMiningSubmit,
		eMethodMiningNotify,
		eMethodClientReconnect
	};

	struct StratumParamMiningSubscribe
	{
		// "params":["equihashminer/0.4b", null,"equihash.eu.nicehash.com","3357"]
		// Received: {"id":1, "error" : null, "result" : [null, "ca1eff67000000000000000067ff1ec95a"]}
		std::string minerVersion;
		std::string sessionID;
		std::string resultSessionID;
		std::string resultNonceLeftpart;

	};

	struct StratumParamMiningAuthorize
	{
		// "params":["164zkRbm23GD2sxar17ZTqYKdT4TcgyW7W.worker3","x"]
		// Received: {"id":2,"result":true,"error":null}
		std::string name;
		std::string password;
	};

	struct StratumParamMiningExtranonceSubscribe
	{
		// "params":[]
	};

	struct StratumParamMiningSetTarget
	{
		// "params":["01e1e1e1e0000000000000000000000000000000000000000000000000000000"]
		std::string target;
	};

	struct StratumParamMiningSetExtranonce
	{
		// "params":["b40d0178000000000000000078010db5d5"]
		std::string extraNonce;
	};

	struct StratumParamMiningSubmit
	{
		// "params":["164zkRbm23GD2sxar17ZTqYKdT4TcgyW7W.worker3","0000003e4b366667","38502758","350000000000000000000000000000",
		// "fd40050049b816d96dd61fa97083775dab360e0d96bc7a9662030388506068798abdb658bdfa9f7df60a1f84361fb2f47305d644e92182d3c492c219a1035e3e60b93cb20ad4e525a35f981daa2a52580e6725515a99c00af97a76ff86c602ffbac3072bc915e8cec05a93fb1b42140de20a5bd3ba4503cfee3cebd93e5f77fbfc0ee9cd6379a78d01a468ab5eecfce2eb693f5cb3e659390c2ad65cb3eb561f2c0aa865ac8f1d4dfa991201fc0fa3e4d12b571c8ca0694ff9c9199f3ab5122f234f42c29598c503e27e06ae56da971de2cdd3cb61039218bde2862ce5c8fa22e2cbbd7f38d6a4f914fb16c31a9b5060114bdf5dd1d4607f956c9d6267676104c102209eb4d853f6da75804e4a266f1fafdd80e80dcad2e4d18d261916b672c968deac263f6bd2d65e0dd37d3fa9c3ef64bfb4e1eafa71d8c9b66c396b131ff3c115d70d806f233bc2c680df7db8be2a891ec40358a898bbc7b66a646807d3dfe5ffc2fd229931bf2284f5f82bdf09b9cbde8685d6ecff71f97df19b0a067f70f46a4cd7d9b94005126e4c5742f06bb9ba7b0d8c3422e68393b4b348195b74d3bcfadaf938a14713595520c57cee53e9e264f655cfafd25b51f4645b6e75fc927eb3b9d1def01756b44f71373da09e42622154fe5ffe9f4a6b5e3f33ec745200255e730f83374cb5d56946f09e31f2b3d75917f7292ac687d7ecba059cb28f73459a91549ee2fea05d6e9e79c35a71654fb545b8c91e2425559a67e2a0ca9abe3043ba5c0c05b00c1ce09dea3d476585d4f777137f6cd05df8504000c33e45b3baafe2f95492ae52834e9cc458c73f10e6b4fd93ea854fbed3b73fc65e6acec155df0245197052f8fbdb28ddde182800a462ad764ee4dabc2b1960a92edbd38087aa1c419638ae729d13df5dcf7d6e1ac6a0abb045f5eb9bbb7d97eafc2717af3acd80007461afd854de91538a40ea612718d04a0b9464eb1813a12f58645b8d605013b733d7a441e0e65f42d90c81fb70ad690329e1abda1cc85faa32a9b3bbed8617c48bd2bf1e81b373b393aecdd138714b6db9617302c185b67e8cf8c9141ea0a0f812df1c9b6312008a144894cc3708f3b8d90af3883a3bdd38fd0c8fead709b9452f190329f49fdad340c5a924357960cc4f860e006aa837093ba951063174060c0541137676691b0246301cca918893177b41e0a1742eff44c19ee8ab06da905f80a650bb55e247d885e4c166550a96866407afe90ef110f48fdfe6b0cabe86edc4abb6e59d6b0f46b9e7a592904f5a62044b6ad83d1541fe5c8376107bb467782919c171a2032f7a490352f124fef68613d22a8b65218213b4f3c225ae480e2d1c85bcf0761c10d42352918789c5eb02107aa10b92a4aafe7170215c6d3a766520d16ef024df35cb1f994735eca78f01e141a5a2a2157141bbb05843fbf9698acffa26261376d3a06ea2d72338e7b72a8352089e56ba1e3c4e04ef03ef8dadb32ba2b0508c333c354ad4745b824d21679c862b08c55b5eeaf6bf445322fa692ebafb1002b12cade94eebfbdca871b6bdc6e909811dfc9aff1afb1a5ebc19f24f4ff3c42381e6e7210fcd92b38c0fe3fdc8f348b8dc7a7f04277c2d525a06399a978c34e10d11d2a0ea2534a594a23137f40a86d0f96e1b0b89315704c4fd45a0f7e3da80dfb34e58a21f266f345efe47f5a7a7b1ba48a5022c41330da8239c66571d0a9f937c189e096424d26296ba5c254f4f956617367ea770fcd93d551192089b6564bae3178a3de585114439a05ea9c245c178762788e7c955ed5054fa77143ca42ef54b8349021688825f5bf9d2565d5b28032e154e97a7dfed213dd8e49ee2dc5dc5438ab71bb83ba1149711aff71dd88a9820fe73888ea102b8ea0f"]}
		// (name, job_id, ntime, nonce_rightpart, sol)
		std::string name;
		std::string job_id;
		std::string ntime;
		std::string nonce_rightpart;
		std::string sol;
	};
	struct StratumParamMiningNotify
	{
		// "params":["0000003e4b366667","04000000","a34bf0bf49c26e8a7646531a107ec2d3bb04149fa5bced400f930e5100000000",
		// "7336ba3437e8d9c0a261f46c0b0221b0f1b67ed76d6222d98ec59d7e94f25890",
		// "0000000000000000000000000000000000000000000000000000000000000000", "38502758", "c875021d", true]
		// [job_id, nVersion, hashPrevBlock, hashMerkleRoot, hashReserved, nTime, nBits, clean_jobs]
		std::string job_id;
		std::string version;
		std::string hashPrevBlock;
		std::string hashMerkleRoot;
		std::string hashReserved;
		std::string time;
		std::string bits;
		bool clean_jobs;
	};
	struct StratumParamClientReconnect
	{
		// "params":[]
	};
	
	struct StratumParam
	{
		StratumParamMiningSubscribe		miningSubscribe;
		StratumParamMiningAuthorize		miningAuthorize;
		StratumParamMiningSetTarget		miningSetTarget;
		StratumParamMiningSetExtranonce miningSetExtranonce;
		StratumParamMiningSubmit		miningSubmit;
		StratumParamMiningNotify		miningNotify;
		StratumParamClientReconnect		clientReconnect;
	};

	struct StratumStruct
	{
		unsigned int Id;
		bool result;
		std::string error;
		//StratumMethod method;
		std::string method;
		StratumParam params;
	};

	void Stratum::Process(char* str)
	{
		char *endptr;
		gason::JsonValue value;
		gason::JsonAllocator allocator;
		int result = jsonParse(str, &endptr, &value, allocator);
		if (result == gason::JSON_PARSE_OK)
		{
//			JSON_NUMBER = 0,
//			JSON_STRING,
//			JSON_ARRAY,
//			JSON_OBJECT,
//			JSON_TRUE,
//			JSON_FALSE,
//			JSON_NULL = 0xF
			StratumStruct message;
			if (value.getTag() == gason::JSON_OBJECT)
			{
				gason::JsonNode* params = nullptr;
				gason::JsonNode* node = value.toNode();
				do 
				{
					if (strcmp(node->key, "id") == 0)
					{
						if (value.getTag() == gason::JSON_NULL)
						{
							message.Id = 0;
						}
						else if (value.getTag() == gason::JSON_NUMBER)
						{
							message.Id = node->value.toInt();
						}
						continue;
					}
					if (strcmp(node->key, "method") == 0)
					{
						message.method = node->value.toString();
						continue;
					}
					if (strcmp(node->key, "params") == 0)
					{
						params = node;
						continue;
					}
				} while (node = node->next);
			}
		}

	}

	void Stratum::MiningSubscribe()
	{

	}

	void Stratum::MiningAuthorize()
	{

	}

	void Stratum::MiningExtranonceSubscribe()
	{

	}

	void Stratum::MiningSubmit()
	{

	}


	void Stratum::OnSetTarget()
	{

	}

	void Stratum::OnSetExtranonce()
	{

	}

	void Stratum::OnResult()
	{

	}

	void Stratum::OnParams()
	{

	}


} // namespace stratum

