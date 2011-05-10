#include <gtk/gtk.h>

static void destroy(GtkWidget *widget, gpointer data) {
  gtk_main_quit();
}

int main(int argc, char *argv[]) {
  GtkWidget *initwindow, *initbox, *namebox, *namelabel, *nameentry;

  gtk_init(&argc, &arv);

  initwindow = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  g_signal_connect(initwindow, "destroy", G_CALLBACK (destroy), NULL);
  initbox = gtk_vbox_new(false, 0);
  gtk_container_add(GTK_CONTAINER (initwindow), initbox);

  namebox = gtk_hbox_new(true, 2);
  namelabel = gtk_label_new("Your name: ");
  nameentry = gtk_entry_new;
  gtk_box_pack_start(namebox, namelabel, false, true, 0);
  gtk_box_pack_start(namebox, nameentry, true, true, 0);





  gtk_widget_show_all(initwindow);
  gtk_main();
  return 0;
}
