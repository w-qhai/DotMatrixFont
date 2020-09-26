#include "mainwindow.h"
#include "ui_mainwindow.h"

QString MainWindow::style_selected = "background: red; border: 1px solid white;";
QString MainWindow::style_unselected = "background: gray; border: 1px solid white;";

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
      _width(8),
      _height(8),
      _pix_size(32),
      _click_left(false),
      _click_right(false)
{
    ui->setupUi(this);
    ui->matrix->resize(16 * this->_pix_size, 16 * this->_pix_size);
    ui->matrix->setStyleSheet("border: 2px solid white;");

    for (int h = 0; h < 16; h++) {
        for (int w = 0; w < 16; w++) {
            _pixs[h][w] = new QLabel(ui->matrix);
            _pixs[h][w]->setStyleSheet(style_unselected);
            _pixs[h][w]->move(w * _pix_size, h * _pix_size);
            _pixs[h][w]->resize(_pix_size, _pix_size);
        }
    }
    ui->matrix_width->setRange(0, 16);
    ui->matrix_height->setRange(0, 16);
    ui->pix_size->setRange(0, _pix_size);
    ui->matrix_width->setValue(_width);
    ui->matrix_height->setValue(_height);
    ui->pix_size->setValue(_pix_size);
    ui->matrix->installEventFilter(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_matrix_width_valueChanged(int width)
{
    this->_width = width;
    paint();
}

void MainWindow::on_matrix_height_valueChanged(int height)
{
    this->_height = height;
    paint();
}

void MainWindow::on_pix_size_valueChanged(int size)
{
    this->_pix_size = size;
    ui->matrix->resize(16 * this->_pix_size, 16 * this->_pix_size);
    paint();
}

void MainWindow::on_output_clicked()
{
    QString output_text;
    int field_width = this->_width > 8 || this->_height > 8 ? 4 : 2;
    for (int w = 0; w < this->_width; w++) {
        QString hex;
        for (int h = 0; h < this->_height; h++) {
            hex = QString::number(_matrix[h][w]) + hex;
        }
        bool ok;
        int res = hex.toInt(&ok, 2);
        output_text += QString("0x%1%2").arg(res, field_width, 16, QChar('0')).arg((w < this->_width - 1 ? ", " : "\n"));
    }
    ui->output_area->setText(output_text);
}

void MainWindow::paint() {
    for (int h = 0; h < 16; h++) {
        for (int w = 0; w < 16; w++) {
            _pixs[h][w]->move(w * _pix_size, h * _pix_size);
            _pixs[h][w]->resize(_pix_size, _pix_size);
            if (h < this->_height && w < this->_width) {
                _pixs[h][w]->show();
                if (this->_matrix[h][w]) {
                    _pixs[h][w]->setStyleSheet(style_selected);
                }
                else {
                    _pixs[h][w]->setStyleSheet(style_unselected);
                }
            }
            else {
                _pixs[h][w]->hide();
                this->_matrix[h][w] = 0;
                _pixs[h][w]->setStyleSheet(style_unselected);
            }
        }
    }
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event) {
    if (watched == ui->matrix) {
        QMouseEvent* mouse_event = reinterpret_cast<QMouseEvent*>(event);
        if (event->type() == QEvent::MouseButtonPress) {

            QPoint pos = mouse_event->pos();
            _last_point = QPoint(pos.x() / _pix_size, pos.y() / _pix_size);
            int x = pos.x() / _pix_size;
            int y = pos.y() / _pix_size;

            if (mouse_event->button() == Qt::LeftButton) {
                _click_left = true;
            }
            else if (mouse_event->button() == Qt::RightButton) {
                _click_right = false;
            }

            if (x >= 0 && x < this->_width && y >= 0 && y < this->_height) {
                this->_matrix[y][x] = mouse_event->button() == Qt::LeftButton;
                paint();
            }
        }
        else if (event->type() == QEvent::MouseButtonRelease) {
            if (mouse_event->button() == Qt::LeftButton) {
                _click_left = false;
            }
            else if (mouse_event->button() == Qt::RightButton) {
                _click_right = false;
            }
        } 
        else if (event->type() == QEvent::MouseMove) {
            QPoint pos = mouse_event->pos();
            int x = pos.x() / _pix_size;
            int y = pos.y() / _pix_size;

            if (_last_point != QPoint(x, y) && x >= 0 && x < this->_width && y >= 0 && y < this->_height) {
                this->_matrix[y][x] = _click_left;
                paint();
            }
            _last_point = QPoint(x, y);
        }
    }
    return QMainWindow::eventFilter(watched, event);
}

void MainWindow::on_clean_clicked()
{
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++) {
            this->_matrix[i][j] = false;
        }
    }
    paint();
}

void MainWindow::on_fill_clicked()
{
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++) {
            this->_matrix[i][j] = true;
        }
    }
    paint();
}
