/*
 * ReportRank.cc
 *
 * Implements the PageRank graph centrality algorithm for word-senses.
 *
 * Copyright (c) 2008 Linas Vepstas <linas@linas.org>
 */
#include <stdio.h>

#include "FollowLink.h"
#include "ForeachWord.h"
#include "ReportRank.h"
#include "Node.h"
#include "SimpleTruthValue.h"
#include "TruthValue.h"

using namespace opencog;

#define DEBUG

ReportRank::ReportRank(void)
{
}

ReportRank::~ReportRank()
{
}

/**
 * For each parse of the sentence, make a report.
 */
void ReportRank::report_rank(Handle h)
{
	parse_cnt = 0;
	foreach_parse(h, &ReportRank::report_parse, this);
}

/**
 * For each parse, walk over each word.
 */
bool ReportRank::report_parse(Handle h)
{
	printf ("Parse %d:\n", parse_cnt);
	foreach_word_instance(h, &ReportRank::report_word, this);
	parse_cnt ++;
	return false;
}

/**
 * Report the parse rank for this word.
 */
bool ReportRank::report_word(Handle h)
{
	// Only noun-senses and verb-senses get ranked.
	std::string pos = get_pos_of_word_instance(h);
	if (pos.compare("#noun") && pos.compare("#verb")) return false;

	double hi_score = 0.0;
	foreach_word_sense_of_inst(h, &ReportRank::choose_sense, this);

	Node *word = dynamic_cast<Node *>(TLB::getAtom(h));
	Node *sense = dynamic_cast<Node *>(TLB::getAtom(hi_scorer));
	printf ("%s sense %s score=%g\n", 
	        word->getName().c_str(), sense->getName().c_str(), hi_score);

	return false;
}

bool ReportRank::choose_sense(Handle word_sense_h,
                              Handle sense_link_h)
{
	Node *n = dynamic_cast<Node *>(TLB::getAtom(sense_link_h));
	double score = n->getTruthValue().getMean();
	if (hi_score < score)
	{
		hi_score = score;
		hi_scorer = word_sense_h;
	}
	return true;
}

/* ============================== END OF FILE ====================== */
