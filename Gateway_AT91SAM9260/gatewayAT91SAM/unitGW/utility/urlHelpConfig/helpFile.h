/*
 * helpFile.h
 *
 *  Created on: 26. mai 2010
 *      Author: Ernad
 */

#ifndef HELPFILE_H_
#define HELPFILE_H_

#include "../../model/gwHelp.h"

char helpFile_init(void);
char gwParamSaver(GW_HELP * pgwHelp);
char helpFile_saveToFile(GW_HELP * pgwHelp);
char helpFile_readFromFile(GW_HELP * pgwHelp);

#endif /* HELPFILE_H_ */
