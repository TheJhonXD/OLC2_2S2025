#include <gtk/gtk.h>

#define COMPILER_CMD "./build/calc"  // La ruta al binario

typedef struct {
    GtkWidget *win;
    GtkTextBuffer *in_buf, *out_buf;
} App;

static void set_output(App *app, const char *txt){
    gtk_text_buffer_set_text(app->out_buf, txt?txt:"", -1);
}
static char* get_input(App *app){
    GtkTextIter a,b;
    gtk_text_buffer_get_bounds(app->in_buf, &a, &b);
    return gtk_text_buffer_get_text(app->in_buf, &a, &b, FALSE); /* g_free luego */
}

// Guarda la entrada en un tmp y ejecuta la calculadora con el tmp como parametro
static void on_run(GtkButton *btn, gpointer u){
    (void)btn; App *app = u;
    char *code = get_input(app);
    gchar *tmp_path=NULL; GError *err=NULL;
    int fd = g_file_open_tmp("temporal.txt", &tmp_path, &err);
    if (fd==-1){ set_output(app, err?err->message:"Error tmp"); if(err)g_error_free(err); g_free(code); return; }
    close(fd);
    if (!g_file_set_contents(tmp_path, code, -1, &err)){
        set_output(app, err?err->message:"Error escribir tmp");
        if(err)g_error_free(err); g_free(tmp_path); g_free(code); return;
    }
    gchar *cmd = g_strdup_printf("%s %s 2>&1", COMPILER_CMD, tmp_path);
    gchar *out=NULL; gint status=0; GError *spawn_err=NULL;
    gboolean ok = g_spawn_command_line_sync(cmd, &out, NULL, &status, &spawn_err);
    if (!ok){ set_output(app, spawn_err?spawn_err->message:"Error al ejecutar"); if(spawn_err)g_error_free(spawn_err); }
    else    { set_output(app, (out && *out)? out : "No hay mensaje de salida"); }
    if(out) g_free(out);
    unlink(tmp_path); g_free(tmp_path); g_free(cmd); g_free(code);
}

static void on_activate(GtkApplication *app, gpointer u){
    (void)u;
    App *A = g_new0(App,1);

    A->win = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(A->win), "IDE JavaLang");
    gtk_window_set_default_size(GTK_WINDOW(A->win), 900, 600);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);
    gtk_container_add(GTK_CONTAINER(A->win), vbox);

    // Botones
    GtkWidget *h = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
    // GtkWidget *b_open = gtk_button_new_with_label("Abrir");
    GtkWidget *b_run  = gtk_button_new_with_label("Ejecutar");
    gtk_box_pack_start(GTK_BOX(h), b_open, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(h), b_run,  FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), h, FALSE, FALSE, 0);

    // Entrada
    GtkWidget *in_view = gtk_text_view_new();
    A->in_buf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(in_view));
    GtkWidget *scr_in = gtk_scrolled_window_new(NULL,NULL);
    gtk_container_add(GTK_CONTAINER(scr_in), in_view);
    gtk_box_pack_start(GTK_BOX(vbox), scr_in, TRUE, TRUE, 0);

    // Salida
    GtkWidget *out_view = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(out_view), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(out_view), FALSE);
    A->out_buf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(out_view));
    GtkWidget *scr_out = gtk_scrolled_window_new(NULL,NULL);
    gtk_container_add(GTK_CONTAINER(scr_out), out_view);
    gtk_box_pack_start(GTK_BOX(vbox), scr_out, TRUE, TRUE, 0);

    g_signal_connect(b_run,  "clicked", G_CALLBACK(on_run),  A);

    gtk_widget_show_all(A->win);
}

int main(int argc, char **argv){
    GtkApplication *app = gtk_application_new("com.example.minigui", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(on_activate), NULL);
    int st = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return st;
}
