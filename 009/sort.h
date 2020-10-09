/**
  * @file sort.h
  * @brief bubble sort
  */
#ifndef SORT_H
#define SORT_H

#include <QObject>

class Sort : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool running READ running WRITE setRunning NOTIFY runningChanged)

private:
    bool m_running;
    int *array;
    int n;

public:
    /**
     * @brief constructor sort
     * @param qty - quantity of elements
     * @param parent
     */
    explicit Sort(int qty, QObject *parent = nullptr);
    /**
     * @brief destructor sort
     */
    ~Sort();
    /**
     * @brief running
     * @return m_running
     */
    bool running() const;
    /**
     * @brief get Array
     * @return array
     */
    int *getArray() const;
    /**
     * @brief get N
     * @return n
     */
    int getN() const;

public slots:
    /**
     * @brief set m_running
     * @param running
     */
    void setRunning(bool running);
    /**
     * @brief run
     */
    void run();

signals:
    /**
     * @brief finished
     */
    void finished();
    /**
     * @brief running changed
     * @param running
     */
    void runningChanged(bool running);
};

#endif // SORT_H
