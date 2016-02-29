#include <iostream>

#include <vector>
#include <boost/shared_ptr.hpp>


#include <iostream>
#include <vector>
#include <boost/shared_ptr.hpp>

#include <ros/ros.h>
#include <tf/transform_listener.h>
#include <tf/transform_broadcaster.h> 

using namespace std;
namespace MohammadTeam
{
	class Player
	{
	    public:

	    Player( string name)
	    {
		 this->name = name;
		// cout << name <<  endl;       
	    }

	     string getTeamName()
		{
	  		return team;
		};

	    int setTeamName( string team)
	    {
		if (team=="red" || team=="green" || team=="blue")
		{
		    this->team = team;
		    return 1;
		}
		else
		{
		     cout << "cannot set team name to " << team <<  endl;
		    return 0;
		}
	    }

	    int setTeamName(int team_index = 0 /*default value*/)
	    {
		switch (team_index)
		{
		    case 0: 
		        return setTeamName("green"); break;
		    case 1: 
		        return setTeamName("green"); break;
		    case 2: 
		        return setTeamName("blue");  break;
		    default: 
		         cout << "wrong team index given. Cannot set team" <<  endl; break;
		}
	    }

	    tf::Transform getPose(void)
            {
                ros::Duration(0.1).sleep(); //To allow the listener to hear messages
                tf::StampedTransform st; //The pose of the player
                try{
                    listener.lookupTransform("/map", name, ros::Time(0), st);
                }
                catch (tf::TransformException ex){
                    ROS_ERROR("%s",ex.what());
                    ros::Duration(1.0).sleep();
                }

                tf::Transform t;
                t.setOrigin(st.getOrigin());
                t.setRotation(st.getRotation());
                return t;
            }
            
	     string name;

	    private:
	       string team;
	       tf::TransformListener listener; 

	};

	class MyPlayer: public Player
	{
	    public: 
            tf::TransformBroadcaster br;

	    MyPlayer( string name,  string team): Player(name)
	    {
		setTeamName(team);
	    
	   tf::Transform t;
            t.setOrigin( tf::Vector3(0.0, 0.0, 0.0) );
            tf::Quaternion q; q.setRPY(0, 0, 0);
            t.setRotation(q);
            br.sendTransform(tf::StampedTransform(t, ros::Time::now(), "/map", name));
            		
	    }
	    
	    void move(double displacement, double turn_angle)
	    {
		
	        double max_d =  1; 
                displacement = (displacement > max_d ? max_d : displacement);

                double min_d =  -0.1; 
                displacement = (displacement > min_d ? min_d : displacement);

                double max_t =  (M_PI/60);
                if (turn_angle > max_t)
                    turn_angle = max_t;
                else if (turn_angle < -max_t)
                    turn_angle = -max_t;

                //Compute the new reference frame
                tf::Transform t_mov;
                t_mov.setOrigin( tf::Vector3(displacement , 0, 0.0) );
                tf::Quaternion q;
                q.setRPY(0, 0, turn_angle);
                t_mov.setRotation(q);

                tf::Transform t = getPose();
                t = t  * t_mov;

                //Send the new transform to ROS
                br.sendTransform(tf::StampedTransform(t, ros::Time::now(), "/map", name));   
	    }
	    int varexample;
	};


	class Team
	{
	    public: 
/*
	    //Team constructor
	    Team(vector<string> player_names)
		{
			for (size_t i=0;i<player_names.size();++i)
			{
  				Player player_temp(player_names[i]);
				players.push_back(player_temp);
			}
		}
	    void printTeamInfo(void)
	    {
		cout << "Team has the following players:" << endl;

		for (size_t i=0; i < players.size(); ++i)
		    cout << players[i].name << endl;
	    }

	    vector<Player> players;
	    //string name;
	    //vector<boost::shared_ptr<Player> > players;
	    */

	};  
}

int main(int argc, char** argv)
{
    // Player player("Mohammad");
    // player.setTeamName("red");
    // player.setTeamName(1);
   /* std::vector<int> as;
     int myint=10;
as.push_back(myint);
*/

   /*
std::vector<string> players;
players.push_back("mohammad");
players.push_back("hamid");
players.push_back("abbas");

MohammadTeam::Team team(players);
team.printTeamInfo();
*/
   
    ros::init(argc, argv, "player_mkasaei_node");
    ros::NodeHandle node;

    //Creating an instance of class MyPlayer
    MohammadTeam::MyPlayer my_player("mkasaei", "red");

    //Infinite loop
    ros::Rate loop_rate(10);
    while (ros::ok())
    {
        //Test the get pose method                                                                                     
        tf::Transform t = my_player.getPose();
        cout << "x = " << t.getOrigin().x() << " y = " << t.getOrigin().y() << endl;
	
	my_player.move(0.2,M_PI/60.0);
	

        ros::spinOnce();
        loop_rate.sleep();
    }

    return 0;




/*    MohammadPlaye::myPlayer player("Messi","blue");

    cout << player.name << endl;     
    cout << player.getTeamName() <<  endl;     
 */
   
    return 1;                                                      
}
