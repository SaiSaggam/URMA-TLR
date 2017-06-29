#ifndef __DIGI_UCI_CMP_H__
#define __DIGI_UCI_CMP_H__

int digi_name_matches_uci_name(char const * const digi_name, char const * const uci_name);
int strlcpy_uci_to_digi(char * const digi, char const * const uci, size_t size);
int strlcpy_digi_to_uci(char * const digi, char const * const uci, size_t size);

#endif /*__DIGI_UCI_CMP_H__*/

