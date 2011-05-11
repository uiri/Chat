#include <gtk/gtk.h>

static void delete(GtkWidget *widget, gpointer data) {
  gtk_main_quit();
}

static void hide(GtkWidget *widget, GtkWidget *initwindow, gpointer data) {
  gtk_widget_hide(initwindow);  
}

static int chat (GtkWidget *widget, GtkTextBuffer *mainbuffer, gpointer data) {
  
}

static void gui(GtkWidget *widget, gchar *name, gpointer data) {
  GtkWidget *mainwindow, *mainbox, *mainscroll,
    *mainview, *mainhbox, *mainmessage, *mainbutton;
  GtkTextBuffer *mainbuffer;
  GtkTextIter *mainiter;
  gchar *bufferchar;
  gint bufferlen;
  gboolean tru, lie;
  int sendstat;
  tru = TRUE;
  lie = FALSE;
  sendstat = 0;

  mainwindow = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  g_signal_connect(mainwindow, "destroy", G_CALLBACK (delete), NULL);
  gtk_window_set_default_size(GTK_WINDOW (mainwindow), 200, 200);
  gtk_window_set_title(GTK_WINDOW (mainwindow), "UI Chat");
  gtk_widget_show(mainwindow);

  mainbox = gtk_vbox_new(lie, 0);
  mainhbox = gtk_hbox_new(lie, 0);
  mainscroll = gtk_scrolled_window_new(NULL, NULL);
  mainbuffer = gtk_text_buffer_new(NULL);
  mainview = gtk_text_view_new_with_buffer(mainbuffer);
  mainmessage = gtk_entry_new();
  mainbutton = gtk_button_new_with_label("Send");

  gtk_container_add(GTK_CONTAINER (mainwindow), mainbox);
  gtk_box_pack_start(GTK_BOX (mainbox), mainscroll, tru, tru, 0);
  gtk_box_pack_start(GTK_BOX (mainbox), mainhbox, lie, lie, 0);
  gtk_box_pack_start(GTK_BOX (mainhbox), mainmessage, tru, tru, 0);
  gtk_box_pack_start(GTK_BOX (mainhbox), mainbutton, lie, lie, 0);
  
  gtk_text_view_set_editable(GTK_TEXT_VIEW (mainview), lie);
  gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW (mainview), GTK_WRAP_WORD_CHAR);
  gtk_container_add(GTK_CONTAINER (mainscroll), mainview);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW (mainscroll), 1, 1);

  g_signal_connect(mainbutton, "chat",

  gtk_widget_show_all(mainwindow);
}

int main(int argc, char *argv[]) {
  GtkWidget *initwindow, *initbox,
    *namebox, *namelabel, *nameentry,
    *buttonbox, *initbutton;
  gboolean tru, lie;
  tru = TRUE;
  lie = FALSE;
  gchar *name;
  
  gtk_init(&argc, &argv);

  initwindow = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  g_signal_connect(initwindow, "destroy", G_CALLBACK (delete), NULL);
  initbox = gtk_vbox_new(lie, 0);
  gtk_container_add(GTK_CONTAINER (initwindow), initbox);

  namebox = gtk_hbox_new(tru, 2);
  namelabel = gtk_label_new("Your name: ");
  nameentry = gtk_entry_new();
  gtk_box_pack_start(GTK_BOX (namebox), namelabel, lie, tru, 0);
  gtk_box_pack_start(GTK_BOX (namebox), nameentry, tru, tru, 0);

  buttonbox = gtk_hbox_new(tru, 2);
  initbutton = gtk_button_new_with_label("CONNECT!");
  gtk_box_pack_start(GTK_BOX (buttonbox), initbutton, tru, tru, 0);

  gtk_box_pack_start(GTK_BOX (initbox), namebox, tru, tru, 0);
  gtk_box_pack_start(GTK_BOX (initbox), buttonbox, tru, tru, 0);

  name = gtk_entry_get_text(GTK_ENTRY (nameentry));
  g_signal_connect(initbutton, "clicked", G_CALLBACK (gui), name);
  g_signal_connect(initbutton, "clicked", G_CALLBACK (hide), initwindow);

  gtk_widget_show_all(initwindow);
  gtk_main();
  return 0;
}
