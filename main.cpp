#include <algorithm>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>

using namespace std;

/*  Declarations
/*   *   *   *   *   *   *   *   *   *   */

typedef struct task {
    int r, p, q, x;
} task_t;

void readTill( string fileHeader, ifstream &data, vector<task_t> &tasks );

int scheduler_std( vector<task_t> &tasks );
int scheduler_alt( vector<task_t> &tasks );


/*  Executor
/*   *   *   *   *   *   *   *   *   *   */

int main( int argc, char **argv ) {

    //  get method STD or ALT
    bool set_ALT = (( string( argv[1] ) == "alt" ) ? true : false );
    bool set_OUT = (( string( argv[1] ) == "out" ) ? true : false );

    //  initial dataset
    vector<task_t> tasks;
    ifstream data( "schr.data.txt" );
    int totalTime = 0;

    //  process for all 4 datasets
    for ( int i = 0; i <= 8 ; i++ ) {

        //  reset tasks
        tasks.clear();

        //  create new file header
        string fileHeader = "data.00" + to_string( i ) + ":";

        //  read data
        readTill( fileHeader, data, tasks );

        //  get partial result
        int result = ( set_ALT ? scheduler_alt( tasks ) : scheduler_std( tasks ));

        //  print result
        cout << endl;
        cout << "---+---+---+---+---+---+---+---+---+---" << endl;

        cout << "Current run: " << i << endl;
        cout << "Calculated time: " << result << endl;


        //  print order
        if( set_OUT ) {

            cout << "Order:" << endl;

            for( int i = 0; i < tasks.size(); i++ ) {
                cout << setw( 2 ) << i + 1 << ". [ r: " << setw( 6 ) << tasks[i].r << " | p: " << setw( 6 ) << tasks[i].p << " | q: " << setw( 6 ) << tasks[i].q << " | x: " << setw( 2 ) << tasks[i].x << "]" << endl;
            }
        }

        cout << "---+---+---+---+---+---+---+---+---+---" << endl;
        cout << endl;

        //  update total result
        totalTime += result;
    }

    //  print result
    cout << endl;
    cout << "---+---+---+---+---+---+---+---+---+---" << endl;

    cout << "<*> Final result <*>" << endl;
    cout << "Calculated total time: " << totalTime << endl;

    cout << "---+---+---+---+---+---+---+---+---+---" << endl;
    cout << endl;

    //  close data stream
    data.close();

    //  final statement
    return 0;
}


/*  Solutions
/*   *   *   *   *   *   *   *   *   *   */

void readTill( string fileHeader, ifstream &data, vector<task_t> &tasks ) {

    //  initial dataset
    string buffer;
    int size;

    //  read till heder found
    do {
        data >> buffer;
    } while( buffer != fileHeader );

    //  read size
    data >> size;

    //  read data
    for( int  i = 0; i < size; i++ ) {

        //  task buffer
        task_t tmp;

        //  read data into task
        data >> tmp.r >> tmp.p >> tmp.q;

        //  set task id
        tmp.x = i;

        //  add task to list
        tasks.push_back( tmp );
    }
}

int scheduler_std( vector<task_t> &tasks ) {

    //  initial dataset
    int m = 0;
    int c = 0;


    /*  scheduling algorithm
    /*   *   *   *   *   *   *   *   */

    //  initial values
    int t = 0;
    vector<task_t> N = tasks;
    vector<task_t> G;
    vector<task_t> K;

    //  initial N set sort
    sort( N.begin(), N.end(), []( task_t const &p, task_t const &q ) -> bool {
        return p.r < q.r;
    });

    //  starts procedure
    while( G.size() || N.size() ) {
        
        //  first step
        while( N.size() && N.front().r <= t ) {

            //  get task
            task_t tmp = N.front();

            //  erase task from N set
            N.erase( N.begin() );

            //  add task to G set
            G.push_back( tmp );
        }

        //  second step
        if( !G.size() ) {

            //  update t value
            t = N.front().r;

        } else {

            //  resort G set
            sort( G.begin(), G.end(), []( task_t const &p, task_t const &q ) -> bool {
                return p.q > q.q;
            });

            //  get task
            task_t tmp = G.front();

            //  erase task from G set
            G.erase( G.begin() );

            //  add task to K set
            K.push_back( tmp );

            //  update t value
            t += tmp.p;
        }
    }

    //  update taksk ref
    tasks = K;

    /*   *   *   *   *   *   *   *   */


    //  recalc values
    for( int i = 0; i < tasks.size(); i++ ) {
        m = max( m, tasks[i].r ) + tasks[i].p;
        c = max( c, m + tasks[i].q );
    }

    //  return result
    return c;
}

int scheduler_alt( vector<task_t> &tasks ) {

    //  initial dataset
    int m = 0;
    int c = 0;


    /*  scheduling algorithm
    /*   *   *   *   *   *   *   *   */

    //  initial values
    int t = 0;
    vector<task_t> N = tasks;
    vector<task_t> G;
    vector<task_t> K;


    //  initial N set sort
    sort( N.begin(), N.end(), []( task_t const &p, task_t const &q ) -> bool {
        return p.r < q.r;
    });

    //  starts procedure
    while( G.size() || N.size() ) {
        
        //  first step
        while( N.size() && N.front().r <= t ) {

            //  get task
            task_t tmp = N.front();

            //  erase task from N set
            N.erase( N.begin() );

            //  add task to G set
            G.push_back( tmp );

            //  test devide
            if( tmp.q > G.front().p ) {

                //  postpone task
                G.front().p = t - tmp.r;

                //  update time
                t = tmp.r;
            }
        }

        //  second step
        if( !G.size() ) {

            //  update t value
            t = N.front().r;

        } else {

            //  resort G set
            sort( G.begin(), G.end(), []( task_t const &p, task_t const &q ) -> bool {
                return p.q > q.q;
            });

            //  get task
            task_t tmp = G.front();

            //  erase task from G set
            G.erase( G.begin() );

            //  add task to K set
            K.push_back( tmp );

            //  update t value
            t += tmp.p;
        }
    }

    //  update taksk ref
    tasks = K;

    /*   *   *   *   *   *   *   *   */


    //  recalc values
    for( int i = 0; i < tasks.size(); i++ ) {
        m = max( m, tasks[i].r ) + tasks[i].p;
        c = max( c, m + tasks[i].q );
    }

    //  return result
    return c;
}