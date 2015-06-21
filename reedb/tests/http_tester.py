import httplib
import json

'''
GET UUID OF THE TARGET VAULT WE WANT TO DEAL WITH!
'''
class RestTester:

	def create_vault(self):
		request = {"name":"default","path":"/home/spacekookie/Desktop","passphrase":"peterpanistderheld"}
		conn = httplib.HTTPConnection("localhost", 55736)
		h = {'Content-Type':'application/json'}
		conn.request("PUT","/vaults", json.dumps(request), h)
		res = conn.getresponse()

		data = {}
		try:
			data = json.loads(res.read())
		except:
			print "[FAILED]", "JSON was not readable for some reason", res.read()
			return
		
		print data['payload']

		self.my_token = data['payload']
		if self.my_token: success = "[PASS]\t"
		else: success = "[FAIL]\t"

		print success, "PUT /vaults", res.status, res.reason, ":'%s'" % data['message']

	def available_vaults(self):
		target_vault = 'default'
		conn = httplib.HTTPConnection("localhost", 55736)
		conn.request("GET","/vaults")
		res = conn.getresponse()

		print "[PASS]\t", "GET /vaults Response", res.status, res.reason
		self.vaults = json.loads(res.read())

		self.uuid = ''
		for k, v in self.vaults['payload'].iteritems():
			if v['name'] == target_vault: self.uuid = k


	''' REQUEST TOKEN '''
	def request_token(self):
		conn = httplib.HTTPConnection("localhost", 55736)
		header = {'Content-Type':'application/json'}
		token_body = {'passphrase':'peterpanistderheld'}
		conn.request("POST","/vaults/%s/request_token" % self.uuid, json.dumps(token_body), header)
		res = conn.getresponse()

		data = None
		try:
			data = json.loads(res.read())
		except:
			print res.read()


		if data and data['success']: success = "[PASS]\t"
		else: success = "[FAIL]\t"

		print success, "POST /vaults/UUID/request_token", res.status, res.reason, ":'%s'" % data['message']
		self.my_token = data['payload']
		print self.my_token


	''' FREE TOKEN '''
	def free_token(self):
		conn = httplib.HTTPConnection("localhost", 55736)
		header = {'Content-Type':'application/json'}
		token_body = {'token':self.my_token}
		conn.request("POST","/vaults/%s/free_token" % self.uuid, json.dumps(token_body), header)
		res = conn.getresponse()
		data = json.loads(res.read())

		if data['success']: success = "[PASS]\t"
		else: success = "[FAIL]\t"
		print success, "POST /vaults/UUID/free_token", res.status, res.reason, ":'%s'" % data['message']
		self.my_token = None


	def get_headers(self):
		conn = httplib.HTTPConnection("localhost", 55736)
		header = {'Content-Type':'application/json'}
		token_body = {'token':self.my_token}
		conn.request("POST","/vaults/%s/headers" % self.uuid, json.dumps(token_body), header)
		res = conn.getresponse()
		data = json.loads(res.read())

		if data['success']: success = "[PASS]\t"
		else: success = "[FAIL]\t"

		print success, "POST /vaults/UUID/headers", res.status, res.reason, ":'%s'" % data['message']
		self.headers = data['payload']

	def close_vault(self):
		conn = httplib.HTTPConnection("localhost", 55736)
		header = {'Content-Type':'application/json'}
		token_body = {'token':self.my_token}
		conn.request("POST","/vaults/%s/close" % self.uuid, json.dumps(token_body), header)
		res = conn.getresponse()
		data = json.loads(res.read())

		if data['success']: success = "[PASS]\t"
		else: success = "[FAIL]\t"

		print success, "POST /vaults/UUID/close", res.status, res.reason, ":'%s'" % data['message']
		self.close_code = data['payload']


	def insert_file(self):
		conn = httplib.HTTPConnection("localhost", 55736)
		header = {'Content-Type':'application/json'}
		token_body = {'name':'MyFace', 'token':self.my_token, 'data':{'body':{'username':'spacekookie', 'passphrase':'abcdefghijklmnop'}, 'header':{'urls':['www.myface.com']}}}
		conn.request("PUT","/vaults/%s/files" % self.uuid, json.dumps(token_body), header)
		res = conn.getresponse()
		data = json.loads(res.read())

		if data['success']: success = "[PASS]\t"
		else: success = "[FAIL]\t"

		self.test_file = data['payload']['file_handle']
		print success, "PUT /vaults/UUID/files", res.status, res.reason, ":'%s'" % data['message']

	def update_file(self):
		print self.my_token

		conn = httplib.HTTPConnection("localhost", 55736)
		header = {'Content-Type':'application/json'}
		token_body = {'token':self.my_token, 'data':{'body':{'note':'This is an awesome social network that combines facebook with myspace. MYFACE!', 'passphrase':'lkj23l4kj23lkj234'}, 'header':{'tags':['social', 'hipster', 'awesome']}}}
		conn.request("POST","/vaults/%s/files/%s" % (self.uuid, self.test_file), json.dumps(token_body), header)
		res = conn.getresponse()

		raw = res.read()
		data = json.loads(raw)

		if data['success']: success = "[PASS]\t"
		else: success = "[FAIL]\t"
		print success, "POST /vaults/UUID/files", res.status, res.reason, ":'%s'" % data['message']


	def get_file_body(self):
		conn = httplib.HTTPConnection("localhost", 55736)
		header = {'Content-Type':'application/json'}
		token_body = {'token':self.my_token}
		conn.request("POST","/vaults/%s/files/%s" % (self.uuid, self.test_file), json.dumps(token_body), header)
		res = conn.getresponse()
		data = json.loads(res.read())

		template = {'header':{'tags':['social', 'hipster', 'awesome'], 'urls':['www.myface.com']}, 'body':{'passphrase':'lkj23l4kj23lkj234', 'note':'This is an awesome social network that combines facebook with myspace. MYFACE!', 'username':'spacekookie'}}

		success = ''
		if data['success']:
			if data == template:
				success = "[PASS]\t"
		else: 
			success = "[FAIL]\t"
		print success, "PUT /vaults/UUID/files", res.status, res.reason, ":'%s'" % data['message']


	def get_file_history(self):
		conn = httplib.HTTPConnection("localhost", 55736)
		header = {'Content-Type':'application/json'}
		token_body = {'token':self.my_token}
		conn.request("POST","/vaults/%s/files/%s/history" % (self.uuid, self.test_file), json.dumps(token_body), header)
		res = conn.getresponse()
		data = json.loads(res.read())

		if data['success']:
			success = "[PASS]\t"
		else: success = "[FAIL]\t"
		print success, "PUT /vaults/UUID/files", res.status, res.reason, ":'%s'" % data['message'], data['payload']


	def __init__(self):
		# Create a vault and get a token
		# self.create_vault()

		# Free the token again to test

		# Get available vaults (self.vaults)
		self.available_vaults()

		# Now get a new token (self.my_token)
		self.request_token()

		# self.free_token()
		# Get headers (fills self.headers)
		# self.get_headers()

		# Insert a new file
		# self.insert_file()

		# Then update it
		# self.update_file()

		# Then return the file and mirror it with what it should be
		# self.get_file_body()

		# Then check the versioning history matches
		# self.get_file_history()

		# Close the vault
		# self.close_vault()


rt = RestTester()