#include <string>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <regex>
#include <queue>
#include <unordered_set>
#include <curl/curl.h>


class Crawler {
	public:
		void crawl( std::string root, std::ofstream& file );
		std::unordered_set<std::string> get_visited_urls() { return visited_urls; }
		void cleanup() { curl_global_cleanup(); }

    private:
      std::unordered_set<std::string> visited_urls;
      const std::regex url{ R"((http[s]?)?://[a-zA-Z0-9\#\-\%\?\=\&\/\_]+(\.[a-zA-Z0-9\#\-\%\?\=\&\/\_]+)*(\.[a-zA-Z0-9\#\%\?\=\&\/\_\-]+)+)", std::regex::extended };
      const std::regex href{ "<\\s*a href\\s*=\\s*[^ ]*" };
      static size_t wrt( void *contents, size_t size, size_t nmemb, std::string *ptr );
      std::string request( std::string url, CURL* curl_ctx );
      void discover_url( std::queue<std::string>& url_array, std::string new_url );
};
