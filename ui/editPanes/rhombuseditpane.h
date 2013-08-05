#ifndef RHOMBUSEDITPANE_H
#define RHOMBUSEDITPANE_H

#include <QObject>
#include <QWidget>
#include <QtGui>
#include "rhombuslabels.h"

class RhombusEditPane : public QWidget
{
    Q_OBJECT
public:
    explicit RhombusEditPane(QVariantMap *variantMap, QVariantMap *variantMapCopy, QWidget *widget, QWidget *parent = 0);

    QWidget *mainWindow;
    QVariantMap *presetMap;
    QVariantMap *presetMapCopy;
    QLabel *paneLabel;
    QLabel *bankDestination;
    QWidget *focusCheck;
    QCheckBox *enableBank;
    QComboBox *bankControl;
    QComboBox *modeMenu;
    QDial *sensitivityDial;
    QWidget *clicked;
    QString prevObjectToLabel;

    void findUiElements();
    void displayRhombusValues();

    int currentPreset;
    int currentRhombus;

    QVariantMap rhombusMap;
    QStringList rhombusParamList;//list of parameter names
    QStringList rhombusUiBlacklist;
    QMap<QString, QSpinBox*> spinBoxMap; //map of spin box address by param name in leftrightParamList

    RhombusLabels *rhombusLabel[1];

    //pointers for velocity check boxes
    QCheckBox *rhombusVelocity;
    QCheckBox *localLEDControl;

    QSpinBox *ledInChannel;
    
signals:
    void signalValueChanged(int presetNum, QString settingsType, QString paramType, int typeNum, QString paramName, int paramValue);
    void signalToLabels(QString);
    
public slots:
    void slotEvents(QString);
    void slotRecallPreset(int);
    void slotValueChanged(int);
    void slotEnable(int);
    void slotBankControl(int);
    void slotToLabels(QString);
    void slotFocusParam(QWidget* oldObject, QWidget* nowObject);
};

#endif // RHOMBUSEDITPANE_H
