
/*!
 * \file main.cc
 * \brief ZIP module for Cracker-ng.
 * \author Mickaël 'Tiger-222' Schoentgen
 * \date 2013.01.27
 *
 * Copyright (C) 2012-2013 Mickaël 'Tiger-222' Schoentgen.
 * See http://www.pkware.com/documents/casestudies/APPNOTE.TXT for
 * more details about ZIP specifications.
 */


#include "./main.h"


// C'est parti mon kiki !
int main(int argc, char *argv[]) {
	std::string filename, input, password;
	size_t flag = NONE;
	arguments argz = {
		MODULE, std::string(VERSION), filename, input, flag, (size_t)argc, argv
	};

	if ( !argz_traitment(argz) ) {
		return 0;
	}
	
	// Signal handler
	signal(SIGABRT, &signal_handler);
	signal(SIGTERM, &signal_handler);
	signal(SIGINT,  &signal_handler);

	// Who I am? I'm a champion!
	Cracker zizi(argz.filename, argz.input);

	size_t str_len = WIDTH - 2;
	char* str = new char[str_len];
	snprintf(str, str_len, "~ %s Cracker-ng v%s { Tiger-222 } ~", MODULE, VERSION);
	zizi.set_title(str);
	snprintf(str, str_len, "%s", functions_ng::basename(argz.filename).c_str());
	zizi.set_file(str);
	snprintf(str, str_len, "%s", functions_ng::basename(argz.input).c_str());
	zizi.set_generator(str);
	delete[] str;

	zizi.set_debug(argz.flag == DEBUG);
#ifdef ZIP
	if ( !zizi.is_ok() ) {
		return 1;
	}
#endif
	zizi.crack();
	return 0;
}

bool argz_traitment(const arguments& argz) {
	if ( argz.argc == 1 ) {
		usage(argz.module);
		return false;
	} else if ( !strcmp(argz.argv[1], "-h") || !strcmp(argz.argv[1], "--help") ) {
		help(argz.module);
		return false;
	} else if ( !strcmp(argz.argv[1], "-v")
			 || !strcmp(argz.argv[1], "--version") ) {
		version(argz.module, argz.version);
		return false;
	} else {
		size_t i;
		bool filename = false;
		bool wordlist = false;
		for ( i = 1; i < argz.argc; ++i ) {
			
			// -f | --file to set the file to crack
			if ( !filename && 
				(!strcmp(argz.argv[i], "-f") || !strcmp(argz.argv[i], "--file")) ) {
				if ( !argz.argv[++i] ) {
					fprintf(stderr, " ! Please gimme a file.\n");
					return false;
				}
				if ( !functions_ng::file_exists(argz.argv[i]) ) {
					fprintf(stderr, " ! Could not open the file.\n");
					return false;
				}
				argz.filename = argz.argv[i];
				filename = true;
			}
			
			// -i | --infos to get informations about a file
			// Useful for the ZIP module, for others it will just do as -f | --file option
			else if ( !filename && 
				     (!strcmp(argz.argv[i], "-i") || !strcmp(argz.argv[i], "--infos")) ) {
				if ( !argz.argv[++i] ) {
					fprintf(stderr, " ! Please gimme a wordlist.\n");
					return false;
				}
				if ( !functions_ng::file_exists(argz.argv[i]) ) {
					fprintf(stderr, " ! Could not open the file.\n");
					return false;
				}
				argz.filename = argz.argv[i];
				argz.flag = DEBUG;
				filename = true;
			}
			
			// -w | --wordlist to set the wordlist file to use
			else if ( !wordlist && 
				    (!strcmp(argz.argv[i], "-w") || !strcmp(argz.argv[i], "--wordlist")) ) {
				if ( !argz.argv[++i] ) {
					fprintf(stderr, " ! Please gimme a wordlist.\n");
					return false;
				}
				if ( !functions_ng::file_exists(argz.argv[i]) ) {
					fprintf(stderr, " ! Could not open the wordlist.\n");
					return false;
				}
				argz.input = argz.argv[i];
				wordlist = true;
			}
		}
	}
	if ( argz.filename.empty() ) {
		fprintf(stderr, " ! Please gimme a file.\n");
		return false;
	} else if ( argz.input.empty() ) {
		argz.input = "STDIN";
	}
	return true;
}

void help(const std::string& module) {
	printf(
		"Copyright (C) 2011-2013 by Mickaël 'Tiger-222' Schoentgen.\n\n"
		"Cracker-ng comes with ABSOLUTELY NO WARRANTY.\n"
		"This is free software, and you are welcome to redistribute it under\n"
		"certain conditions. See the GNU General Public Licence for details.\n\n"
		"Cracker-ng, a multiple file cracker.\n\n");
	usage(module);
	printf(
		"\nAvailable options:\n"
		"    -f, --file     file to crack\n"
		"    -i, --infos    print informations about a file to crack (only for ZIP)\n"
		"    -,  --stdin    read from STDIN\n"
		"    -w, --wordlist dictionnary tu use\n"
		"    -h, --help     display this message\n"
		"    -v, --version  display module version\n\n"
		"Do not hesitate to contact me at <tiger-222@matriux.com> for critics,\n"
		"suggestions, contributions (or whatever you want!).\n");
}

void signal_handler(int s) {
	UNUSED(s);
	this_is_now_we_fight = false;
}

void usage(const std::string& module) {
	std::string mo = module;
	transform(mo.begin(), mo.end(), mo.begin(), ::tolower);
	printf(
		"Usage:\n"
		"\t%scracker-ng -f <file> [-w <wordlist>]\n"
		"\tgenerator | %scracker-ng -f <file> -\n"
		"Where generator could be cat, crunch, john, jot or whatever you want.\n",
		mo.c_str(), mo.c_str());
}

void version(const std::string& module, const std::string& version) {
	printf("%s Cracker-ng version %s.\n", module.c_str(), version.c_str());
}
