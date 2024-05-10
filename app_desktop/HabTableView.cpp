#include "HabTableView.hpp"

#include "Constants.hpp"
#include "HabitDelegate.hpp"
#include "HabitDialog.hpp"
#include "HabitModel.hpp"
#include "SoundPlayer.hpp"
#include "Utility.hpp"

#include <QDebug>
#include <QInputDialog>
#include <QMouseEvent>
#include <numeric>

HabTableView::HabTableView(QWidget* parent)
    : QTableView{parent}
{
}

void HabTableView::setHabitDelegate(HabitDelegate* delegate)
{
    QAbstractItemView::setItemDelegate(delegate);
    mHabitDelegate = delegate;
}

void HabTableView::mousePressEvent(QMouseEvent* event)
{
    auto indx = indexAt(event->pos());
    if (!indx.isValid() || indx.column() == 0)
    {
        return;
    }

    if (toInt(indx.data(HabitModel::HabitStatusRole)) != INT_NULL_VALUE &&
        static_cast<Habit::Type>(toInt(indx.data(HabitModel::TypeRole))) !=
            Habit::Type::Quantitative)
    {
        mHabitDelegate->startAnimation(indx);
    }
    qDebug() << "You pressed on cell with row=" << indx.row()
             << " column=" << indx.column();
}

void HabTableView::mouseReleaseEvent(QMouseEvent*)
{
    mHabitDelegate->stopAnimation();
    qDebug() << "Mouse Released!";
}

void HabTableView::mouseMoveEvent(QMouseEvent*)
{
}

void HabTableView::mouseDoubleClickEvent(QMouseEvent* event)
{
    auto indx = indexAt(event->pos());
    if (!indx.isValid())
    {
        return;
    }

    Habit::Type habitType =
        static_cast<Habit::Type>(toInt(indx.data(HabitModel::TypeRole)));
    if (indx.column() != 0 && habitType == Habit::Type::OneTime)
    {
        return;
    }

    if (indx.column() == 0)
    {
        QString habitName = toQString(indx.data(HabitModel::Roles::NameRole));
        QDate	habitDate = toQDate(indx.data(HabitModel::Roles::StartDateRole));
        QString habitUnits = toQString(indx.data(HabitModel::Roles::UnitsRole));
        int		habitDailyGoal = toInt(indx.data(HabitModel::Roles::DailyGoalRole));

        HabitDialog habitDialog{"Edit habit", HabitDialog::Mode::Edit, "Accept", "Delete",
            habitName, habitDate, habitType, habitUnits, habitDailyGoal};

        decltype(auto) result = habitDialog.exec();
        if (result == QDialog::Accepted)
        {
            QString		name = habitDialog.getName();
            QDate		date = habitDialog.getStartDate();
            Habit::Type type = habitDialog.getType();
            QString		units = habitDialog.getUnits();
            int			dailyGoal = habitDialog.getDailyGoal();

            model()->setData(indx, name, HabitModel::Roles::NameRole);
            model()->setData(indx, date, HabitModel::Roles::StartDateRole);
            model()->setData(indx, static_cast<int>(type), HabitModel::Roles::TypeRole);
            model()->setData(indx, units, HabitModel::Roles::UnitsRole);
            model()->setData(indx, dailyGoal, HabitModel::Roles::DailyGoalRole);
        }
        else if (result == QDialog::Rejected)
        {
            qDebug() << "Rejected!";
            model()->removeRow(indx.row());
        }
    }
    else
    {
        bool	ok;
        QString units = toQString(indx.data(HabitModel::UnitsRole));
        int		currentUnitsAmount = toInt(indx.data(HabitModel::HabitStatusRole));
        int		dailyGoal = toInt(indx.data(HabitModel::DailyGoalRole));

        int newUnitsAmount = QInputDialog::getInt(this, tr("Set habit amount"), units,
            currentUnitsAmount, 0, std::numeric_limits<int>::max(), 1, &ok);
        if (ok)
        {
            if (newUnitsAmount >= dailyGoal)
            {
                SoundPlayer::playSound(SoundPlayer::Type::Success);
            }
            model()->setData(indx, newUnitsAmount, HabitModel::HabitStatusRole);
        }
    }
}
