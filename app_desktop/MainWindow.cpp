#include "MainWindow.hpp"

#include "HabTableView.hpp"
#include "HabitDelegate.hpp"
#include "HabitDialog.hpp"
#include "HabitModel.hpp"
#include "ImportDialog.hpp"
#include "StatusProxyModel.hpp"
#include "ui_MainWindow.h"

#include <QDockWidget>
#include <QFileDialog>
#include <QHeaderView>
#include <QLabel>
#include <QMouseEvent>
#include <QPushButton>
#include <QStandardItem>
#include <QVBoxLayout>
#include <QVariant>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow{parent}
    , ui{new Ui::MainWindow}
{
    ui->setupUi(this);

    HabitModel*		  habitModel = new HabitModel{false, this};
    StatusProxyModel* statusModel = new StatusProxyModel{this};
    statusModel->setSourceModel(habitModel);

    QVBoxLayout*  mainLayout = new QVBoxLayout{this};
    HabTableView* tableView = new HabTableView{this};
    QPushButton*  addHabitButton = new QPushButton{"Add habit", this};
    QPushButton*  importButton = new QPushButton{"Import habits from .db file", this};

    tableView->setModel(statusModel);
    tableView->setHabitDelegate(new HabitDelegate{tableView});
    tableView->setSelectionMode(QAbstractItemView::NoSelection);
    tableView->setFocusPolicy(Qt::NoFocus);
    tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableView->setShowGrid(false);
    tableView->verticalHeader()->setVisible(false);
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    connect(addHabitButton, &QPushButton::clicked, this,
        [habitModel]()
        {
            HabitDialog habitDialog{
                "New habit", HabitDialog::Mode::Add, "Create", "Discard"};
            if (habitDialog.exec() == QDialog::Accepted)
            {
                QString			  name = habitDialog.getName();
                QDate			  date = habitDialog.getStartDate();
                Habit::Type		  type = habitDialog.getType();
                QString			  units = habitDialog.getUnits();
                int				  dailyGoal = habitDialog.getDailyGoal();
                Habit::RepeatInfo repeatInfo = habitDialog.getRepeatInfo();

                habitModel->addHabit(
                    Habit{name, date, type, units, dailyGoal, repeatInfo});
            }
        });

    connect(importButton, &QPushButton::clicked, this,
        [this, habitModel]()
        {
            QString filename = QFileDialog::getOpenFileName(this,
                tr("Select import .db File"), "/home/jonathan/", tr("Databases (*.db)"));

            if (filename.isEmpty())
            {
                return;
            }
            //
            ImportDialog impDialog{filename, "Approve"};
            if (impDialog.exec() == QDialog::Accepted)
            {
                const std::vector<Habit>& habits = impDialog.getNewHabits();
                for (auto& habit : habits)
                {
                    habitModel->addHabit(habit);
                }
            }
        });

    mainLayout->addWidget(tableView);
    mainLayout->addWidget(addHabitButton);
    mainLayout->addWidget(importButton);
    centralWidget()->setLayout(mainLayout);
}

MainWindow::~MainWindow()
{
    delete ui;
}
