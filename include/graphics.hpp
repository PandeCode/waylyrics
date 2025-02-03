#include <epoxy/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <gtk/gtk.h>
#include <math.h>
#include <stdio.h>

using glm::lookAt;
using glm::mat4;
using glm::perspective;
using glm::rotate;
using glm::vec3;


static constexpr unsigned int WIDTH = 480;
static constexpr unsigned int HEIGHT = 270;

inline GLchar *VERTEX_SOURCE =
#include "../shaders/vert.glsl"
;

inline GLchar *FRAGMENT_SOURCE =
#include "../shaders/frag.glsl"
;

static constexpr GLfloat vertex_data[] =
#include "../shaders/vertex_data"
;

static GtkWidget *gl_area = NULL;

static long current_frame = 0.0;
static float mouse_x = 0.0f;
static float mouse_y = 0.0f;
static long delta_time = 0.0;
static GDateTime *last_frame;
static int dt = 0;

static GLuint position_buffer;
static GLuint program;
static GLuint vao;

static mat4 model = mat4(1.0);

static gboolean on_mouse_move(GtkWidget *widget, GdkEventMotion *event,
                              gpointer user_data) {
  mouse_x = event->x;
  mouse_y = event->y;
  return TRUE;
}

/* Create and compile a shader */
static GLuint create_shader(int type) {
  GLuint shader;
  int status;
  shader = glCreateShader(type);
  if (type == GL_FRAGMENT_SHADER) {
    glShaderSource(shader, 1, &FRAGMENT_SOURCE, NULL);
    g_signal_connect(gl_area, "motion-notify-event", G_CALLBACK(on_mouse_move),
                     NULL);
    gtk_widget_add_events(gl_area, GDK_POINTER_MOTION_MASK);
  }

  if (type == GL_VERTEX_SHADER) {
    glShaderSource(shader, 1, &VERTEX_SOURCE, NULL);
  }
  glCompileShader(shader);

  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
  if (status == GL_FALSE) {
    int log_len;
    char *buffer;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_len);
    buffer = (char *)g_malloc(log_len + 1);
    glGetShaderInfoLog(shader, log_len, NULL, buffer);
    g_warning("Compile failure in %s shader:\n%s",
              type == GL_VERTEX_SHADER ? "vertex" : "fragment", buffer);
    g_free(buffer);
    glDeleteShader(shader);
    return 0;
  }

  return shader;
}

/* We need to set up our state when we realize the GtkGLArea widget */
static void realize(GtkWidget *widget) {
  GdkGLContext *context;
  gtk_gl_area_make_current(GTK_GL_AREA(widget));
  if (gtk_gl_area_get_error(GTK_GL_AREA(widget)) != NULL)
    return;
  context = gtk_gl_area_get_context(GTK_GL_AREA(widget));

  /* We only use one VAO, so we always keep it bound */
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  /* This is the buffer that holds the vertices */
  glGenBuffers(1, &position_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, position_buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data,
               GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  GLuint vertex, fragment;
  int status;
  vertex = create_shader(GL_VERTEX_SHADER);

  if (vertex == 0) {
    return;
  }

  fragment = create_shader(GL_FRAGMENT_SHADER);

  if (fragment == 0) {
    glDeleteShader(vertex);
    return;
  }

  program = glCreateProgram();
  glAttachShader(program, vertex);
  glAttachShader(program, fragment);

  glLinkProgram(program);

  glGetProgramiv(program, GL_LINK_STATUS, &status);
  if (status == GL_FALSE) {
    int log_len;
    char *buffer;

    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_len);

    buffer = (char *)g_malloc(log_len + 1);
    glGetProgramInfoLog(program, log_len, NULL, buffer);

    g_warning("Linking failure:\n%s", buffer);

    g_free(buffer);

    glDeleteProgram(program);
    program = 0;

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return;
  }

  glDetachShader(program, vertex);
  glDetachShader(program, fragment);

  glEnable(GL_CULL_FACE);
  glFrontFace(GL_CCW);
  glCullFace(GL_BACK);
  glEnable(GL_DEPTH_TEST);
}

/* We should tear down the state when unrealizing */
static void unrealize(GtkWidget *widget) {
  gtk_gl_area_make_current(GTK_GL_AREA(widget));

  if (gtk_gl_area_get_error(GTK_GL_AREA(widget)) != NULL)
    return;

  glDeleteBuffers(1, &position_buffer);
  glDeleteProgram(program);
}

static void draw_box(long delta_time) {
  /* Use our shaders */
  glUseProgram(program);

  model = rotate(model, (float)delta_time / 1000, vec3(1, 1, 0));
  glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE,
                     &model[0][0]);
  vec3 position = vec3(0, 0, 5);
  vec3 front = vec3(0, 0, -1);
  vec3 up = vec3(0, 1, 0);
  mat4 view = lookAt(position, position + front, up);
  glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_FALSE,
                     &view[0][0]);
  mat4 projection =
      perspective(45.0, double(WIDTH) / double(HEIGHT), 0.1, 100.0);

  glUniform1f(glGetUniformLocation(program, "time"),
              (float)current_frame / 1000000.0);
  glUniform2f(glGetUniformLocation(program, "resolution"), WIDTH, HEIGHT);
  glUniform2f(glGetUniformLocation(program, "mouse"), mouse_x, mouse_y);

  glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE,
                     &projection[0][0]);

  glBindVertexArray(vao);
  /* Use the vertices in our buffer */

  /* Draw the three vertices as a triangle */
  glDrawArrays(GL_TRIANGLES, 0, 36);

  /* We finished using the buffers and program */
  glBindVertexArray(0);
  glDisableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glUseProgram(0);
}

static gboolean render(GtkGLArea *area, GdkGLContext *context) {
  GDateTime *date_time;

  date_time = g_date_time_new_now_local();
  current_frame = g_date_time_get_microsecond(date_time);
  delta_time = g_date_time_difference(date_time, last_frame) / 1000;
  last_frame = date_time;

  if (gtk_gl_area_get_error(area) != NULL)
    return FALSE;

  /* Clear the viewport */
  glClearColor(0.0, 0.0, 0.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  /* Draw our object */
  draw_box(delta_time);
  /* Flush the contents of the pipeline */
  glFlush();
  gtk_gl_area_queue_render(area);
  return TRUE;
}

inline void addArea(void *gl_widget) {
  GtkWidget *box;
  box = gtk_box_new(GTK_ORIENTATION_VERTICAL, FALSE);
  g_object_set(box, "margin", 12, NULL);
  gtk_box_set_spacing(GTK_BOX(box), 100);
  gtk_container_add(GTK_CONTAINER(gl_widget), box);
  gl_area = gtk_gl_area_new();
  gtk_widget_set_size_request(gl_area, WIDTH, HEIGHT);
  gtk_box_pack_start(GTK_BOX(box), gl_area, 1, 1, 0);

  g_signal_connect(gl_area, "realize", G_CALLBACK(realize), NULL);
  g_signal_connect(gl_area, "unrealize", G_CALLBACK(unrealize), NULL);

  g_signal_connect(gl_area, "render", G_CALLBACK(render), NULL);
}
