#include <fstream>
#include "crawler/crawler.h"

int main( int argc, char *argv[] ) {

	

	std::ofstream file;
	file.open( "visited_urls" );

	Crawler crawler;
	crawler.crawl( "https://www.theguardian.com/uk", file );
	crawler.crawl( "https://www.runescape.com", file );
	crawler.crawl( "bbc.co.uk", file );

	file.close();
	
	crawler.cleanup();

	
}
