# -*- coding:utf-8 -*-
# file:client.py
#

import Tkinter
import socket
import sys

class Window:
	clientSocket = -1
	
	def __init__(self, root):
		label1 = Tkinter.Label(root, text = 'IP')
		label2 = Tkinter.Label(root, text = "Port")
		label3 = Tkinter.Label(root, text = 'Data')
		
		label1.place(x = 5, y = 5)
		label2.place(x = 5, y = 30)
		label3.place(x = 5, y = 55)
		
		self.entryIP = Tkinter.Entry(root)
		self.entryIP.insert(Tkinter.END, '127.0.0.1')
		self.entryPort = Tkinter.Entry(root)
		self.entryPort.insert(Tkinter.END, '10086')
		self.entryData = Tkinter.Entry(root)
		self.entryData.insert(Tkinter.END, 'Hello')
		self.Recv = Tkinter.Text(root)
		self.entryIP.place(x = 40, y = 5)
		self.entryPort.place(x = 40, y = 30)
		self.entryData.place(x = 40, y = 55)
		self.Recv.place(y = 115)
		
		self.send = Tkinter.Button(root, text = '发送数据', command = self.Send)
		self.send.place(x = 40, y = 80)
		
		self.connect = Tkinter.Button(root, text = '连接', command = self.Connect)
		self.connect.place(x = 110, y = 80)

		self.close = Tkinter.Button(root, text = '关闭', command = self.Close)
		self.close.place(x = 150, y = 80)		
		
	def Send(self):
		try:
			data = self.entryData.get()
			self.clientSocket.send(data)
			rdata = self.clientSocket.recv(1024)			
			self.Recv.insert(Tkinter.END, 'Server:' + rdata + '\n')
		except socket.error, arg:
			self.Recv.insert(Tkinter.END, '发送错误\n')
			#(errno, err_msg) = arg
			#print "Send Fail:%s error = %d " % (err_msg, errno)
	
	def Connect(self):
		t = self.Recv
		
		try:
			ip = self.entryIP.get()
			port = int(self.entryPort.get())			
			self.clientSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
			self.clientSocket.connect((ip,port))
			self.clientSocket.settimeout(10);
			self.Recv.insert(Tkinter.END, '连接成功\n')
		except socket.error, arg:
			t.insert(Tkinter.END, self.entryIP.get() + " " + self.entryPort.get())
			self.Recv.insert(Tkinter.END, '连接失败\n')
			(errno, err_msg) = arg
			print "Connect server failed:%s, errno = %d" % (err_msg, errno)
	
	def Close(self):
		self.clientSocket.close()
		
root = Tkinter.Tk()
window = Window(root)
root.mainloop()