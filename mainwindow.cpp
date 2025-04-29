#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , studentCount(0)
{
    ui->setupUi(this);

    // تعيين عنوان النافذة وحجمها
    setWindowTitle("نظام إدارة درجات الطلاب");
    resize(800, 600);

    // إعداد القوائم
    setupMenus();

    // إعداد واجهة المستخدم
    setupUI();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupMenus()
{
    // قائمة ملف
    QMenu *fileMenu = menuBar()->addMenu("ملف");

    QAction *saveAction = new QAction("حفظ", this);
    saveAction->setShortcut(QKeySequence::Save);
    connect(saveAction, &QAction::triggered, this, &MainWindow::on_actionSave_triggered);
    fileMenu->addAction(saveAction);

    fileMenu->addSeparator();

    QAction *exitAction = new QAction("خروج", this);
    exitAction->setShortcut(QKeySequence::Quit);
    connect(exitAction, &QAction::triggered, this, &MainWindow::on_actionExit_triggered);
    fileMenu->addAction(exitAction);

    // قائمة طالب
    QMenu *studentMenu = menuBar()->addMenu("طالب");

    QAction *addStudentAction = new QAction("إضافة طالب جديد", this);
    addStudentAction->setShortcut(QKeySequence("Ctrl+N"));
    connect(addStudentAction, &QAction::triggered, this, &MainWindow::on_actionAddStudent_triggered);
    studentMenu->addAction(addStudentAction);

    QAction *showAllAction = new QAction("عرض جميع الطلاب", this);
    connect(showAllAction, &QAction::triggered, this, &MainWindow::on_actionShowAll_triggered);
    studentMenu->addAction(showAllAction);

    // قائمة مساعدة
    QMenu *helpMenu = menuBar()->addMenu("مساعدة");

    QAction *aboutAction = new QAction("حول البرنامج", this);
    connect(aboutAction, &QAction::triggered, this, &MainWindow::on_actionAbout_triggered);
    helpMenu->addAction(aboutAction);
}

void MainWindow::setupUI()
{
    // إنشاء العنصر المركزي
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // تخطيط رئيسي
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    // عنوان البرنامج
    QLabel *titleLabel = new QLabel("نظام إدارة درجات الطلاب", this);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(16);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    // جدول عرض بيانات الطلاب
    tableWidget = new QTableWidget(this);
    tableWidget->setColumnCount(4);
    tableWidget->setHorizontalHeaderLabels({"الاسم", "الرقم التعريفي", "المعدل", "التقدير"});
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableWidget->setAlternatingRowColors(true);
    tableWidget->horizontalHeader()->setStretchLastSection(true);
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    mainLayout->addWidget(tableWidget);

    // تخطيط الأزرار
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    // زر إضافة طالب
    addButton = new QPushButton("إضافة طالب جديد", this);
    connect(addButton, &QPushButton::clicked, this, &MainWindow::addStudent);
    buttonLayout->addWidget(addButton);

    // زر عرض التقارير
    displayButton = new QPushButton("عرض التقارير", this);
    connect(displayButton, &QPushButton::clicked, this, &MainWindow::displayReport);
    buttonLayout->addWidget(displayButton);

    mainLayout->addLayout(buttonLayout);

    // حالة البرنامج
    statusLabel = new QLabel("جاهز - عدد الطلاب: 0", this);
    statusBar()->addWidget(statusLabel);
}

void MainWindow::on_actionExit_triggered()
{
    close();
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this, "حول البرنامج",
                       "نظام إدارة درجات الطلاب\n"
                       "الإصدار 1.0\n"
                       "برنامج بسيط لإدارة درجات الطلاب لطلاب السنة الأولى");
}

void MainWindow::on_actionSave_triggered()
{
    QMessageBox::information(this, "حفظ", "تم حفظ البيانات بنجاح!");
    // هنا يمكن إضافة كود لحفظ البيانات في ملف
}

void MainWindow::on_actionAddStudent_triggered()
{
    addStudent();
}

void MainWindow::on_actionShowAll_triggered()
{
    displayReport();
}

void MainWindow::addStudent()
{
    if (studentCount >= MAX_STUDENTS) {
        QMessageBox::warning(this, "تحذير", "تم الوصول إلى الحد الأقصى لعدد الطلاب!");
        return;
    }

    // إنشاء حوار لإدخال بيانات الطالب
    QDialog dialog(this);
    dialog.setWindowTitle("إضافة طالب جديد");
    dialog.setMinimumWidth(300);

    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    QFormLayout *formLayout = new QFormLayout();

    // حقول الإدخال
    QLineEdit *nameEdit = new QLineEdit(&dialog);
    formLayout->addRow("اسم الطالب:", nameEdit);

    QLineEdit *idEdit = new QLineEdit(&dialog);
    idEdit->setText(QString::number(10000 + studentCount)); // قيمة مقترحة فقط
    formLayout->addRow("الرقم التعريفي:", idEdit);

    QComboBox *subjectCountCombo = new QComboBox(&dialog);
    subjectCountCombo->addItem("3 مواد", 3);
    subjectCountCombo->addItem("4 مواد", 4);
    subjectCountCombo->addItem("5 مواد", 5);
    formLayout->addRow("عدد المواد:", subjectCountCombo);

    QSpinBox *gradeSpin[MAX_SUBJECTS];
    for (int i = 0; i < MAX_SUBJECTS; i++) {
        gradeSpin[i] = new QSpinBox(&dialog);
        gradeSpin[i]->setRange(0, 100);
        gradeSpin[i]->setValue(70); // قيمة افتراضية
        formLayout->addRow(QString("درجة المادة %1:").arg(i + 1), gradeSpin[i]);

        // إخفاء حقول الدرجات الزائدة في البداية
        if (i >= 3) {
            gradeSpin[i]->setVisible(false);
            formLayout->labelForField(gradeSpin[i])->setVisible(false);
        }
    }

    // تحديث عدد حقول الدرجات المرئية عند تغيير عدد المواد
    connect(subjectCountCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index) {
        int count = subjectCountCombo->itemData(index).toInt();
        for (int i = 0; i < MAX_SUBJECTS; i++) {
            bool visible = i < count;
            gradeSpin[i]->setVisible(visible);
            formLayout->labelForField(gradeSpin[i])->setVisible(visible);
        }
    });

    layout->addLayout(formLayout);

    // أزرار الحوار
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    layout->addWidget(buttonBox);

    // عرض الحوار وانتظار النتيجة
    if (dialog.exec() == QDialog::Accepted) {
        // تحقق من إدخال الاسم
        if (nameEdit->text().trimmed().isEmpty()) {
            QMessageBox::warning(this, "خطأ", "يجب إدخال اسم الطالب!");
            return;
        }

        // الحصول على بيانات الطالب
        studentNames[studentCount] = nameEdit->text();
        bool ok;
        int idValue = idEdit->text().toInt(&ok);
        if (!ok) {
            QMessageBox::warning(this, "خطأ", "الرقم التعريفي يجب أن يكون رقمًا!");
            return;
        }
        studentIDs[studentCount] = idValue;

        int subjectCount = subjectCountCombo->currentData().toInt();
        int grades[MAX_SUBJECTS] = {0};

        for (int i = 0; i < subjectCount; i++) {
            grades[i] = gradeSpin[i]->value();
        }

        // حساب المعدل والتقدير
        studentAverages[studentCount] = calculateAverage(grades, subjectCount);
        studentGrades[studentCount] = determineGrade(studentAverages[studentCount]);

        // زيادة عداد الطلاب
        studentCount++;

        // تحديث الجدول وحالة البرنامج
        updateTable();
        statusLabel->setText(QString("جاهز - عدد الطلاب: %1").arg(studentCount));

        QMessageBox::information(this, "نجاح", "تمت إضافة الطالب بنجاح!");
    }
}

float MainWindow::calculateAverage(int grades[], int size)
{
    int sum = 0;
    for (int i = 0; i < size; i++) {
        sum += grades[i];
    }
    return static_cast<float>(sum) / size;
}

char MainWindow::determineGrade(float average)
{
    if (average >= 90) return 'A';
    else if (average >= 80) return 'B';
    else if (average >= 70) return 'C';
    else if (average >= 60) return 'D';
    else return 'F';
}

void MainWindow::updateTable()
{
    tableWidget->setRowCount(studentCount);

    for (int i = 0; i < studentCount; i++) {
        tableWidget->setItem(i, 0, new QTableWidgetItem(studentNames[i]));
        tableWidget->setItem(i, 1, new QTableWidgetItem(QString::number(studentIDs[i])));
        tableWidget->setItem(i, 2, new QTableWidgetItem(QString::number(studentAverages[i], 'f', 2)));
        tableWidget->setItem(i, 3, new QTableWidgetItem(QString(studentGrades[i])));
    }
}

void MainWindow::displayReport()
{
    if (studentCount == 0) {
        QMessageBox::information(this, "تقرير", "لا توجد بيانات للطلاب!");
        return;
    }

    // البحث عن أعلى وأدنى معدل
    float highest = studentAverages[0];
    float lowest = studentAverages[0];

    for (int i = 1; i < studentCount; i++) {
        if (studentAverages[i] > highest)
            highest = studentAverages[i];
        if (studentAverages[i] < lowest)
            lowest = studentAverages[i];
    }

    // إنشاء نص التقرير
    QString report = "تقرير الطلاب:\n\n";
    report += "عدد الطلاب: " + QString::number(studentCount) + "\n";
    report += "أعلى معدل: " + QString::number(highest, 'f', 2) + "\n";
    report += "أدنى معدل: " + QString::number(lowest, 'f', 2) + "\n\n";

    // عرض التقرير
    QMessageBox::information(this, "تقرير الطلاب", report);
}
