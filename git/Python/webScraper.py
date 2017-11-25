#ProgramName:   webScraper.py
#Author:    Faris Abdel-Maksoud
#Description:   Experimenting with webscrapers to generate a .txt file
#               .txt files to be processed by NTLK/Scikit separately

#CurrentIssues: -Some websites are protected against scraping
#               by things like Cloudflare.
#               -Need to figure out how to retrieve images and their text.

from bs4 import BeautifulSoup
import urllib

url = urllib.urlopen('https://www.fordham.edu').read()

soup = BeautifulSoup(url, 'html.parser')

#get text
print(soup.get_text())

#get hyperlinks
for link in soup.find_all('a'):
    #print(link.get('href'))

f = open("scrape.txt", "w")
f.write(soup.get_text().encode('utf-8'))
f.close()
