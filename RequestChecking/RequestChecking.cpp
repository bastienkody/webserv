#include"RequestChecking.hpp"


static	int CheckBasics(Request rq)
{
	if (rq.getRql().getUrl().getPath().size() == 0) // empty path in rql --> bad syntax
		return 400;
	if (rq.getRql().getIsVersionGood() == false) // version not supported --> 505
		return 505;
	if (rq.getRql().getIsVerbGood() == false) // method not implemented --> 501 (server connait que get, post and delete. 405 sera envoye si on connait la methode mais ne lautorise pas via configfile)
		return 401;
}

