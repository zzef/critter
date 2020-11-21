#include <string>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <curl/curl.h>
#include <regex>
#include <queue>
#include <unordered_map>
#include <fstream>

static size_t wrt(void *contents, size_t size, size_t nmemb, std::string *ptr) {
	
	char *data = (char*) contents;
	size_t real_size = size * nmemb;
	*ptr += std::string(data); //This may cause problems if string has \0

	return real_size;
}

static std::string request(std::string url, CURL* curl_ctx) {

	std::string response = "";

	struct curl_slist *list = NULL; 

	//std::string host = "Host: ";
	//host += url;	

	if(curl_ctx) {

		//list = curl_slist_append(list, host.c_str());
		//list = curl_slist_append(list, "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:82.0) Gecko/20100101 Firefox/82.0");
		//list = curl_slist_append(list, "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8");
		//list = curl_slist_append(list, "Accept-Language: en-US,en;q=0.5");
		//list = curl_slist_append(list, "Accept-Encoding: gzip, deflate, br");
		//list = curl_slist_append(list, "DNT: 1");
		//list = curl_slist_append(list, "Connection: keep-alive");
		//list = curl_slist_append(list, "Upgrade-Insecure-Requests: 1");
		
		//curl_easy_setopt(curl_ctx, CURLOPT_VERBOSE, 1L);
  		curl_easy_setopt(curl_ctx, CURLOPT_HTTPHEADER, list);
		curl_easy_setopt(curl_ctx, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl_ctx, CURLOPT_WRITEFUNCTION, wrt);
		curl_easy_setopt(curl_ctx, CURLOPT_WRITEDATA, &response);
        curl_easy_setopt(curl_ctx, CURLOPT_FOLLOWLOCATION, 1L);
		CURLcode res = curl_easy_perform(curl_ctx);
		curl_easy_cleanup(curl_ctx);

		curl_slist_free_all(list);
	}
	return response;

}

std::unordered_map<std::string,bool> visited;
std::regex url (
	R"((http[s]?)?://[a-zA-Z0-9\#\-\%\?\=\&\/\_]+(\.[a-zA-Z0-9\#\-\%\?\=\&\/\_]+)*(\.[a-zA-Z0-9\#\%\?\=\&\/\_\-]+)+)",
	std::regex::extended
);
std::regex href("<\\s*a href\\s*=\\s*[^ ]*");

void crawl(std::string root, std::ofstream& file) {

	std::queue<std::string> urls;
	urls.push(root);
	visited[root]=true;
	
	while(!urls.empty()) {
		
		CURL* curl = curl_easy_init();
		std::string response = request(urls.front(), curl);
		file << urls.front() << std::endl;
		std::cout << "= site " << urls.front() << " ============================================================" << std::endl;
		std::cout << response << std::endl;	
		std::cout << "===================================================================" << std::endl;
		std::smatch cm;
		std::smatch cm1;
	
		std::string tmp;

		while (std::regex_search(response,cm,href)) {	
			tmp = cm[0];
			//std::cout << "[" << tmp << "]" << std::endl;
			while(std::regex_search(tmp,cm1,url)) {
				std::string new_url = cm1[0];
				std::cout<< "[" << new_url << "]" << std::endl;
  				std::unordered_map<std::string,bool>::const_iterator got = visited.find(new_url);
				if (got == visited.end()) {
					urls.push(new_url);
					visited[new_url]=true;
				}
				tmp = cm1.suffix().str();
			}
			response = cm.suffix().str();
		}
	
		urls.pop();
		
	}

}

int main(int argc, char *argv[]) {

	std::ofstream file;
	file.open("visited");
	
	crawl("https://www.theguardian.com/uk",file);
	crawl("https://www.runescape.com",file);
	crawl("bbc.co.uk",file);

	file.close();

	curl_global_cleanup();
	
}
