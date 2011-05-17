/*This file is part of UI Chat.

UI Chat is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

UI is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with UI.  If not, see <http://www.gnu.org/licenses/>. */

#include <gtk/gtk.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>

static void delete(GtkWidget *widget, gpointer data) {
  gtk_main_quit();
}

static int client(int domain, int type, int protocol, struct sockaddr *serv_addr, int addrlen) {
  int returnvalue;
  returnvalue = socket(domain, type, protocol);
  connect(returnvalue, serv_addr, addrlen);
  return returnvalue;
}

static int server(int sock, int domain, int type, int protocol, struct sockaddr *serv_addr, int addrlen, int backlog, struct sockaddr_storage client_addr, socklen_t addr_size) {
  int returnvalue;
  sock = socket(domain, type, protocol);
  bind(sock, serv_addr, addrlen);
  listen(sock, backlog);
  addr_size = sizeof client_addr;
  returnvalue = accept(sock, (struct sockaddr *)&client_addr, &addr_size);
  return returnvalue;
}

static void *recvdata (GList *list) {
  char *recvmessage;
  recvmessage = malloc(256 * sizeof(char));
  int *sock;
  sock = g_list_nth_data(list, 0);
  GtkTextBuffer *mainbuffer;
  mainbuffer = g_list_nth_data(list, 1);  
  while (1) {
    GtkTextIter iter[2];
    char *text;
    text = malloc(25900 * sizeof(char));
    recv(*sock, recvmessage, 256, 0);
    /* If else based on whether it is info or message */
    strcat(recvmessage, "\n");
    gtk_text_buffer_get_iter_at_offset(mainbuffer, &iter[0], 0);
    gtk_text_buffer_get_iter_at_offset(mainbuffer, &iter[1], -1);
    text = gtk_text_buffer_get_text(mainbuffer, &iter[0], &iter[1], TRUE);
    strcat(text, recvmessage);
    gtk_text_buffer_set_text(mainbuffer, text, -1);
  }


  /*gchar *text, *message;
  GtkTextIter iter[2];
  GtkTextBuffer *mainbuffer;
  char *buffer;
  buffer = malloc(256 * sizeof(char));
  GError **ohgod;
  int messagesize;
  gsize *readsize;
  int *sock = g_list_nth_data(list, 4);

  mainbuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(g_list_nth_data(list, 0)));
  gtk_text_buffer_get_iter_at_offset(mainbuffer, &iter[0], 0);
  gtk_text_buffer_get_iter_at_offset(mainbuffer, &iter[1], -1);

  /*messagesize = g_io_channel_get_buffer_size(clientio);
  readsize = &messagesize;*/
  /*recv(*sock, buffer, 256, 0);
    
  message = buffer;
  text = g_strconcat(gtk_text_buffer_get_text(mainbuffer, &iter[0], &iter[1], TRUE), message, NULL);
  gtk_text_buffer_set_text(mainbuffer, text, -1);*/
}

static int senddata (GtkWidget *widget, GList *list, gpointer data) {
  char *text, *name, *sendmessage, *message, *mebuffer, *buffer;
  text = malloc(25900 * sizeof(char));
  name = malloc(32 * sizeof(char));
  sendmessage = malloc(300 * sizeof(char));
  message = malloc(256 * sizeof(char));
  mebuffer = malloc(256 * sizeof(char));
  buffer = malloc(256 * sizeof(char));
  size_t mematch;
  mematch = (3 * sizeof (char));
  /*char *recv_buffer;
    recv_buffer = malloc(256 * sizeof(char));*/
  gdouble clamp;
  GtkTextIter iter[2];
  GtkTextBuffer *mainbuffer;
  int *sock = g_list_nth_data(list, 4);
 

  message = gtk_entry_buffer_get_text(gtk_entry_get_buffer(GTK_ENTRY(g_list_nth_data(list, 2))));
  name = gtk_label_get_text(GTK_LABEL(g_list_nth_data(list, 1)));
  mainbuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(g_list_nth_data(list, 0)));
  gtk_text_buffer_get_iter_at_offset(mainbuffer, &iter[0], 0);
  gtk_text_buffer_get_iter_at_offset(mainbuffer, &iter[1], -1);
  buffer = gtk_text_buffer_get_text(mainbuffer, &iter[0], &iter[1], TRUE);

  /*sendmessage = g_strconcat( gtk_label_get_text(GTK_LABEL(g_list_nth_data(list, 1))),": ",
                         gtk_entry_get_text(GTK_ENTRY(g_list_nth_data(list, 2))), "\n", "\0", NULL);

  text = g_strconcat(gtk_text_buffer_get_text(mainbuffer, &iter[0], &iter[1], TRUE), sendmessage, NULL);
  gtk_entry_set_text(GTK_ENTRY(g_list_nth_data(list, 2)), "");
  gtk_text_buffer_set_text(mainbuffer, text, -1);
  send(*sock, buffer, 256, 0);*/

  /*if (message == "/quit") {
    strcpy(sendmessage, name);
    strcat(sendmessage, ": QUIT");
    /*     if for when there are no sockets left 
    } else {*/
  if ((strncmp(message, "/me", mematch)) == 0) {
    mebuffer = strtok(message, " ");
    mebuffer = strtok(NULL, "");
    message = "";
    strcpy(sendmessage, "*");
    strcat(sendmessage, name);
    strcat(sendmessage, mebuffer);
  } else {
    strcpy(sendmessage, name);
    strcat(sendmessage, ": ");
    strcat(sendmessage, message);
  }
  send(*sock, sendmessage, 256, 0);
  strcpy(text, buffer);
  strcat(text, sendmessage);
  strcat(text, "\n");
  gtk_text_buffer_set_text(mainbuffer, text, -1);

  clamp = gtk_adjustment_get_upper((g_list_nth_data(list, 3)));
  gtk_adjustment_set_value((g_list_nth_data(list, 3)), clamp);
  gtk_entry_set_text(GTK_ENTRY(g_list_nth_data(list, 2)), "");
}

static void gui(GtkWidget *widget, GList *initlist, gpointer data) {
  GtkWidget *mainwindow, *mainbox, *mainscroll, *mainbutton, *mainhbox, *mainview, *mainmessage, *mainlabel;
  GtkTextBuffer *mainbuffer;
  /*GtkTextIter iter[2];*/
  GtkAdjustment *vertadjust;
  GIOChannel *clientio;
  GList *mainlist, *recvlist;
  gboolean tru, lie;
  pthread_t recvthread;
  int sendstat, backlog;
  gboolean isclient;
  /*  char *buffer;
      buffer = malloc(256 * sizeof(char));*/
  backlog=10;
  isclient=gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(g_list_nth_data(initlist, 3)));
  mainlist=NULL; recvlist=NULL;
  tru = TRUE;
  lie = FALSE;
  const gchar *port, *ip;
  port = malloc(10 * sizeof(char));
  ip = malloc(20 * sizeof(char));
  sendstat = 0;
  port = gtk_entry_get_text(GTK_ENTRY(g_list_nth_data(initlist, 2)));
  if (isclient) 
    ip = gtk_entry_get_text(GTK_ENTRY(g_list_nth_data(initlist, 4)));

  /* Begin Berkeley Socket Stuff */

  struct sockaddr_storage client_addr;
  socklen_t addr_size;
  struct addrinfo hints, *res;
  int sock, client_sock, listencount;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  if (isclient) {
    getaddrinfo( ip, port, &hints, &res);
  } else {
    hints.ai_flags = AI_PASSIVE;
    getaddrinfo(NULL, port, &hints, &res);
  }

  if (isclient) {
    client_sock = client(res->ai_family, res->ai_socktype, res->ai_protocol, res->ai_addr, res->ai_addrlen);
    listencount = 0;
  } else {
    client_sock = server(sock, res->ai_family, res->ai_socktype, res->ai_protocol, res->ai_addr, res->ai_addrlen, backlog, client_addr, addr_size);
    listencount = 1;
  }

  recvlist = g_list_append(recvlist, &client_sock);
  recvlist = g_list_append(recvlist, mainbuffer);
  pthread_create(&recvthread, NULL, recvdata, recvlist);

  /* Back to your regularly scheduled GUI stuff */

  gtk_widget_hide(g_list_nth_data(initlist, 0));
  mainwindow = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  g_signal_connect(mainwindow, "destroy", G_CALLBACK (delete), NULL);
  gtk_window_set_default_size(GTK_WINDOW (mainwindow), 400, 400);
  gtk_window_set_title(GTK_WINDOW (mainwindow), "UI Chat");
  gtk_widget_show(mainwindow);

  mainbox = gtk_vbox_new(lie, 0);
  mainhbox = gtk_hbox_new(lie, 0);
  mainscroll = gtk_scrolled_window_new(NULL, NULL);
  mainbuffer = gtk_text_buffer_new(NULL);
  mainview = gtk_text_view_new_with_buffer(mainbuffer);
  mainlabel = gtk_label_new(gtk_entry_get_text(GTK_ENTRY(g_list_nth_data(initlist, 1))));
  mainmessage = gtk_entry_new();
  mainbutton = gtk_button_new_with_label("Send");
  vertadjust = gtk_scrolled_window_get_vadjustment(GTK_SCROLLED_WINDOW(mainscroll));

  gtk_container_add(GTK_CONTAINER (mainwindow), mainbox);
  gtk_box_pack_start(GTK_BOX (mainbox), mainscroll, tru, tru, 0);
  gtk_box_pack_start(GTK_BOX (mainbox), mainhbox, lie, lie, 0);
  gtk_box_pack_start(GTK_BOX (mainhbox), mainlabel, lie, lie, 0);
  gtk_box_pack_start(GTK_BOX (mainhbox), mainmessage, tru, tru, 0);
  gtk_box_pack_start(GTK_BOX (mainhbox), mainbutton, lie, lie, 0);
  
  gtk_text_view_set_editable(GTK_TEXT_VIEW (mainview), lie);
  gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW (mainview), GTK_WRAP_WORD_CHAR);
  gtk_container_add(GTK_CONTAINER (mainscroll), mainview);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW (mainscroll), 1, 1);
  /*gtk_text_buffer_set_text(mainbuffer, first, -1);*/

  /*  recv(client_sock, buffer, 256, 0);
  gtk_text_buffer_get_iter_at_offset(mainbuffer, &iter[0], 0);
  gtk_text_buffer_get_iter_at_offset(mainbuffer, &iter[1], -1);

  message = buffer;
  text = g_strconcat(gtk_text_buffer_get_text(mainbuffer, &iter[0], &iter[1], TRUE), message, NULL);
  gtk_text_buffer_set_text(mainbuffer, text, -1);*/

  mainlist = g_list_append(mainlist, mainview);
  mainlist = g_list_append(mainlist, mainlabel);
  mainlist = g_list_append(mainlist, mainmessage);
  mainlist = g_list_append(mainlist, vertadjust);
  mainlist = g_list_append(mainlist, &client_sock);
  g_signal_connect(mainbutton, "clicked", G_CALLBACK (senddata), mainlist);

  gtk_widget_show_all(mainwindow);
}

int main(int argc, char *argv[]) {
  GtkWidget *initwindow, *initbox,
    *namebox, *namelabel, *nameentry,
 /* *firstbox, *firstlabel, *firstentry,*/
    *radiobox, *clientradio, *serverradio,
    *ipbox, *iplabel, *ipentry,
    *portbox, *portlabel, *portentry,
    *buttonbox, *initbutton;
  GList *initlist, *radiolist;
  gboolean tru, lie;
  tru = TRUE;
  lie = FALSE;
  initlist=NULL; radiolist=NULL;
  
  gtk_init(&argc, &argv);

  initwindow = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW (initwindow), "UI Chat Initialization");
  g_signal_connect(initwindow, "destroy", G_CALLBACK (delete), NULL);
  initbox = gtk_vbox_new(lie, 0);
  gtk_container_add(GTK_CONTAINER (initwindow), initbox);

  namebox = gtk_hbox_new(tru, 2);
  namelabel = gtk_label_new("Your name: ");
  nameentry = gtk_entry_new();
  gtk_box_pack_start(GTK_BOX (namebox), namelabel, lie, tru, 0);
  gtk_box_pack_start(GTK_BOX (namebox), nameentry, tru, tru, 0);

  /*firstbox = gtk_hbox_new(tru, 2);
  firstlabel = gtk_label_new("First message: ");
  firstentry = gtk_entry_new();
  gtk_box_pack_start(GTK_BOX (firstbox), firstlabel, lie, tru, 0);
  gtk_box_pack_start(GTK_BOX (firstbox), firstentry, tru, tru, 0);*/

  radiobox = gtk_hbox_new(tru, 2);
  serverradio = gtk_radio_button_new_with_label(NULL, "Server");
  clientradio = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(serverradio), "Client");
  gtk_box_pack_start(GTK_BOX (radiobox), clientradio, lie, tru, 0);
  gtk_box_pack_start(GTK_BOX (radiobox), serverradio, lie, tru, 0);

  ipbox = gtk_hbox_new(tru, 2);
  iplabel = gtk_label_new("IP Address to connect to: ");
  ipentry = gtk_entry_new();
  gtk_box_pack_start(GTK_BOX (ipbox), iplabel, lie, tru, 0);
  gtk_box_pack_start(GTK_BOX (ipbox), ipentry, tru, tru, 0);

  portbox = gtk_hbox_new(tru, 2);
  portlabel = gtk_label_new("Port: ");
  portentry = gtk_entry_new();
  gtk_box_pack_start(GTK_BOX (portbox), portlabel, lie, tru, 0);
  gtk_box_pack_start(GTK_BOX (portbox), portentry, tru, tru, 0);

  buttonbox = gtk_hbox_new(tru, 2);
  initbutton = gtk_button_new_with_label("CONNECT!");
  gtk_box_pack_start(GTK_BOX (buttonbox), initbutton, tru, tru, 0);

  gtk_box_pack_start(GTK_BOX (initbox), namebox, tru, tru, 0);
  /*gtk_box_pack_start(GTK_BOX (initbox), firstbox, tru, tru, 0);*/
  gtk_box_pack_start(GTK_BOX (initbox), radiobox, tru, tru, 0);
  gtk_box_pack_start(GTK_BOX (initbox), ipbox, tru, tru, 0);
  gtk_box_pack_start(GTK_BOX (initbox), portbox, tru, tru, 0);
  gtk_box_pack_start(GTK_BOX (initbox), buttonbox, tru, tru, 0);

  initlist = g_list_append(initlist, initwindow);
  initlist = g_list_append(initlist, nameentry);
  /*initlist = g_list_append(initlist, firstentry);*/
  initlist = g_list_append(initlist, portentry);
  initlist = g_list_append(initlist, clientradio);
  initlist = g_list_append(initlist, ipentry);
  g_signal_connect(initbutton, "clicked", G_CALLBACK (gui), initlist);

  gtk_widget_show_all(initwindow);
  gtk_main();
  return 0;
}
