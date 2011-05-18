# -*- coding: utf-8 -*-
# Copyright 2011 Uiri Noyb
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
# You may contact Uiri Noyb via electronic mail with the address uiri@compucrunch.com

require 'gtk2'
require 'socket'

def delete
  Gtk.main_quit
  false
end

def groupchat (server, sockets)
  socket = sockets[sockets.length]
  socket = server.accept
  $introarray.each{|initmessage|
    Thread.new { socket.send(initmessage, 0) }
  }
  sockets.push(socket)
  Thread.new { recvchat(socket) }
  Thread.new { groupchat(server, sockets) }
end

def recvchat (socket)
  while 1
    recvmessage = socket.recv(256)
    if recvmessage =~ /^You /
      $introarray.push(recvmessage)
    else
      recvmessage = recvmessage + "\n"
    end
    if recvmessage =~ /QUIT/
      removedex = $sockets.index(socket) + 1
      $introarray.delete($introarray[removedex])
      $sockets.delete(socket)
      message = recvmessage.gsub(/QUIT\n$/, '').gsub(/\:/, '')
      buffer = $mainbuffer.text
      recvmessage = message + "has left the chat"
      $mainbuffer.set_text(buffer + recvmessage + "\n")
      $logfile.write(recvmessage + "\n")
      $mainscroll.vadjustment.set_value($mainscroll.vadjustment.upper)
      if $sockets == []
        buffer = $mainbuffer.text
        $mainbuffer.set_text(buffer + "No one is left to communicate with. Hit enter or the send button to close this program.\n")
        $logfile.write("No one is left to communicate with. Hit enter or the send button to close this program.\n")
      else
        $sockets.each {|sock|
          if sock != socket
            sock.send(recvmessage, 0)
          end
        }
      end
      break
    else
      buffer = $mainbuffer.text
      $mainbuffer.set_text(buffer + recvmessage)
      $logfile.write(recvmessage)
      $mainscroll.vadjustment.set_value($mainscroll.vadjustment.upper)
      $sockets.each {|sock|
        if sock != socket
          unless recvmessage =~ /^You /
            recvmessage = recvmessage.gsub(/\n/, '')  
          end
          sock.send(recvmessage, 0)
        end
      }
    end
  end
end

def newmessage(buffer, message, name, sockets)
  if message == '/quit'
    sendmessage = name + ": QUIT"
    if sockets != []
      sockets.each {|socket|
        socket.send(sendmessage, 0)
      }
    end
    $mainwindow.destroy
    Gtk.main_quit
  else
    if message =~ /^\/me/
      message = message.gsub(/^\/me/, '')
      sendmessage = "*" + name + message
    else
      sendmessage = name + ": " + message
    end
    if sockets != []
      sockets.each {|socket|
        Thread.new { socket.send(sendmessage, 0) }
      }
      $mainbuffer.set_text(buffer + sendmessage + "\n")
      $logfile.write(sendmessage)
      $mainscroll.vadjustment.set_value($mainscroll.vadjustment.upper)
      $mainmessage.set_text('')
    else
      $mainwindow.destroy
      Gtk.main_quit
    end
  end
end

initwindow = Gtk::Window.new
initwindow.signal_connect('delete_event') do delete end
initbox = Gtk::VBox.new(false, 0)
initwindow.add(initbox)

namebox = Gtk::HBox.new(true, 2)
namelabel = Gtk::Label.new('Your name:')
nameentry = Gtk::Entry.new
namebox.pack_start(namelabel, false, true, 0)
namebox.pack_start(nameentry, true, true, 0)

radiobox = Gtk::HBox.new(true, 2)
serverradio = Gtk::RadioButton.new("Server")
clientradio = Gtk::RadioButton.new(serverradio, "Client")
radiobox.pack_start(clientradio, false, true, 0)
radiobox.pack_start(serverradio, false, true, 0)

portbox = Gtk::HBox.new(true, 2)
portlabel = Gtk::Label.new('Port:')
portentry = Gtk::Entry.new
portbox.pack_start(portlabel, false, true, 0)
portbox.pack_start(portentry, true, true, 0)

ipbox = Gtk::HBox.new(true, 2)
iplabel = Gtk::Label.new('IP Address to connect to:')
ipentry = Gtk::Entry.new
ipbox.pack_start(iplabel, false, true, 0)
ipbox.pack_start(ipentry, true, true, 0)

buttonbox = Gtk::HBox.new(true, 2)
initbutton = Gtk::Button.new('CONNECT!')
buttonbox.pack_start(initbutton, true, true, 0)

initbox.pack_start(namebox, true, true, 0)
initbox.pack_start(radiobox, true, true, 0)
initbox.pack_start(portbox, true, true, 0)
initbox.pack_start(ipbox, true, true, 0)
initbox.pack_start(buttonbox, true, true, 0)

initbutton.signal_connect( "clicked" ) do
  name = nameentry.text
  $introarray = []
  port = portentry.text
  ip = ipentry.text
  isclient = clientradio.active?
  $sockets = []
  initwindow.destroy
  $mainwindow = Gtk::Window.new('RubyChat')
  $mainwindow.signal_connect('delete_event') do delete end
  $mainwindow.set_default_size(500,500)
  $mainwindow.show
  mainbox = Gtk::VBox.new(false, 0)
  $mainscroll = Gtk::ScrolledWindow.new
  $mainbuffer = Gtk::TextBuffer.new
  mainview = Gtk::TextView.new($mainbuffer)
  mainhbox = Gtk::HBox.new(false, 0)
  $mainmessage = Gtk::Entry.new
  mainbutton = Gtk::Button.new('Send')
  message = "You are now talking with " + name + "\n"

  if isclient == true
    $sockets[0] = TCPSocket.new(ip, port)
  else
    serv = TCPServer.new(port)
    $sockets[0] = serv.accept
    Thread.new { groupchat(serv, $sockets) }
  end

  $introarray.push(message)
  $sockets[0].send(message, 0)
  Thread.new { recvchat($sockets[0]) }

  t = Time.now
  $logfile = File.new(Dir.home + "rubychat-" + name + t.year + "-" + t.month + "-" + t.day + ".log", "a")
  if $logfile
    $mainbuffer.set_text_("Logging...")
  end

  mainbutton.signal_connect( "clicked" ) do
    newmessage($mainbuffer.text, $mainmessage.text, name, $sockets)
  end

  $mainmessage.signal_connect( "activate" ) do
    newmessage($mainbuffer.text, $mainmessage.text, name, $sockets)
  end

  $mainwindow.add(mainbox)
  $mainwindow.set_resizable(true)
  mainview.set_editable(false)
  mainview.wrap_mode = Gtk::TextTag::WRAP_WORD_CHAR
  mainhbox.pack_start($mainmessage, true, true, 0)
  mainhbox.pack_start(mainbutton, false, false, 0)
  $mainscroll.add(mainview)
  $mainscroll.set_policy(1,1)
  mainbox.pack_start($mainscroll, true, true, 0)
  mainbox.pack_start(mainhbox, false, false, 0)
  $mainwindow.show_all
end

initwindow.show_all
Gtk.main
