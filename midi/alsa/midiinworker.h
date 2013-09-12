/* 
 * File:   MidiInWorker.h
 * Author: jrussell
 *
 * Created on September 11, 2013, 9:15 PM
 */

#ifndef MIDIINWORKER_H
#define	MIDIINWORKER_H
#include <QThread>
class MidiInWorker : public QObject{
    Q_OBJECT
public:
    MidiInWorker();
    MidiInWorker(const MidiInWorker& orig);
    virtual ~MidiInWorker();
signals:
void sysexRecieved();
public slots:
    //Do the work
    void work();
    void connect(int client, int port);
    void disconnect();
private:
};

#endif	/* MIDIINWORKER_H */

