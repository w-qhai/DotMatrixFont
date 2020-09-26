#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include <QDebug>
#include <QLabel>
#include <QEvent>
#include <QMouseEvent>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_matrix_width_valueChanged(int arg1);

    void on_matrix_height_valueChanged(int arg1);

    void on_pix_size_valueChanged(int arg1);

    void on_output_clicked();

    void on_clean_clicked();

    void on_fill_clicked();

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    void paint();

private:

    Ui::MainWindow *ui;
    int _width;
    int _height;
    int _pix_size;

    bool _matrix[16][16] = {{ false }};
    QLabel* _pixs[16][16];

    bool _click_left;
    bool _click_right;
    static QString style_selected;
    static QString style_unselected;
    QPoint _last_point;
};
#endif // MAINWINDOW_H
