#ifndef STRATUM_H_
#define STRATUM_H_

//#include "gason/src/gason.h"

namespace stratum
{

	class Stratum
	{
	public:
		void Process(char* str);
		void Process(const char* str);

		void MiningSubscribe();
		void MiningAuthorize();
		void MiningExtranonceSubscribe();
		void MiningSubmit();
		void MiningNotify();
		void ClientReconnect();

		void OnSetTarget();
		void OnSetExtranonce();
		void OnResult();
		void OnParams();
	};

} // namespace stratum

#endif // STRATUM_H_