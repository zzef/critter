#include "crawler.h"
#include <fstream>

size_t Crawler::wrt( void *data, size_t size, size_t nmemb, void *userp ) {

   std::cout << "loading data" << std::endl;
	
   size_t realsize = size * nmemb;
   struct memory *mem = (struct memory *)userp;
 
   char *ptr = (char*) realloc(mem->response, mem->size + realsize + 1);
   if(ptr == NULL)
     return 0;  /* out of memory! */
 
   mem->response = ptr;
   memcpy(&(mem->response[mem->size]), data, realsize);
   mem->size += realsize;
   mem->response[mem->size] = 0;


   return realsize;
}

struct memory Crawler::request( std::string url, CURL* curl_ctx ) {

	struct memory chunk;
	chunk.response = (char*) malloc(1);
	chunk.size = 0;
	struct curl_slist *list = NULL; 

	//std::string host = "Host: ";
	//host += url;	

	if( curl_ctx ) {

		//list = curl_slist_append( list, host.c_str(  ) );
		//list = curl_slist_append( list, "User-Agent: Mozilla/5.0 ( X11; Linux x86_64; rv:82.0 ) Gecko/20100101 Firefox/82.0" );
		//list = curl_slist_append( list, "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8" );
		//list = curl_slist_append( list, "Accept-Language: en-US,en;q=0.5" );
		//list = curl_slist_append( list, "Accept-Encoding: gzip, deflate, br" );
		//list = curl_slist_append( list, "DNT: 1" );
		//list = curl_slist_append( list, "Connection: keep-alive" );
		//list = curl_slist_append( list, "Upgrade-Insecure-Requests: 1" );
		
		//curl_easy_setopt( curl_ctx, CURLOPT_VERBOSE, 1L );
		curl_easy_setopt( curl_ctx, CURLOPT_CONNECTTIMEOUT, 20L );
		curl_easy_setopt( curl_ctx, CURLOPT_TIMEOUT, 20L );
  		curl_easy_setopt( curl_ctx, CURLOPT_HTTPHEADER, list );
		curl_easy_setopt( curl_ctx, CURLOPT_URL, url.c_str(  ) );
		curl_easy_setopt( curl_ctx, CURLOPT_WRITEFUNCTION, wrt );
		curl_easy_setopt( curl_ctx, CURLOPT_WRITEDATA, (void*)&chunk );
		curl_easy_setopt( curl_ctx, CURLOPT_FOLLOWLOCATION, 1L );
		CURLcode res = curl_easy_perform( curl_ctx );
		curl_easy_cleanup( curl_ctx );

		curl_slist_free_all( list );
	}
	return chunk;

}

void Crawler::discover_url( std::queue<std::string>& url_queue, std::string new_url ) {
	
	std::unordered_set<std::string>::const_iterator visited_checker = visited_urls.find( new_url );
	if ( visited_checker == visited_urls.end() ) {
		std::cout<< "[" << new_url << "]" << std::endl;
		url_queue.push( new_url );
		visited_urls.insert(new_url);
	}
}

void Crawler::crawl( std::string root, std::ofstream& file ) {

	std::queue<std::string> url_queue;


	Crawler::visited_urls.insert( root );
	url_queue.push( root );
	
	while( !url_queue.empty() ) {
		
		CURL* curl = curl_easy_init();
		std::cout << "ws: " << url_queue.front() << std::endl;
		struct memory m = request(url_queue.front(),curl);
		std::cout << m.size << std::endl;
		std::cout << (char*) m.response << std::endl;
		std::cout << "copying: " << url_queue.front() << std::endl;
		std::string response = std::string(m.response);

		std::cout << "downloading: " << url_queue.front() << std::endl;
		file << url_queue.front() << std::endl;
		std::cout << "submitted: " << url_queue.front() << std::endl;

		std::smatch href_match;
		std::smatch url_match;
		std::string curr_href;

		// Loop through all href tags in the requested page
		while (std::regex_search( response, href_match, href ) ) {	
			curr_href = href_match[0];
			//std::cout << curr_href << std::endl;

			// Search for the url in the href
			std::regex_search( curr_href, url_match, url );

			// Add the url to the list of unexplored URL's if not already explored
			if( url_match.ready() ) {
				std::string new_url = url_match[0];
				Crawler::discover_url( url_queue, new_url );
			}

			response = href_match.suffix().str();
		}
	
		url_queue.pop();

	}
}

