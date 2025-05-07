#!/usr/bin/python3
# A web client in Python to connect to the SWEE admin server for
# automatically download device logs. In particular, I want to see if I can
# download the CO2 data from the NZ units. This data is obtained from a SWEE
# device's web page on the server as follows:
#   - Click 'Logs'
#   - View full logs
#   - Go to the 'eye' at the end of the log entry and click on it
#   - The dialog box that opens will contain the CO2 data

import requests
from requests.auth import AuthBase

LOGIN_URL = 'http://admin.swee.co/login'
AUTH_PAGE = 'http://admin.swee.co/swee/group-management/update?id=383'

payload = {
	'Login[username]': '<email>',
	'Login[password]': '<password>',
	'Login[rememberMe]': '0'
}

# with requests.Session() as session:
# 	p = session.post(LOGIN_URL, data=payload)
# 	print(p.text)
# 	#r = session.get(AUTH_PAGE)
# 	#print(r.text)

r = requests.post(LOGIN_URL, data=payload)
print(r.content)
