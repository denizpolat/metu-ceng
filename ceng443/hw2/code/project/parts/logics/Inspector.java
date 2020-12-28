package project.parts.logics;

import project.SimulationRunner;
import project.components.Robot;
import project.utility.Common;
import project.utility.SmartFactoryException;

import java.lang.reflect.Field;
import java.util.List;

public class Inspector extends Logic
{
    @Override public void run ( Robot robot )
    {
        // TODO



        // The inspector checks each worker robot to see if it has lost
        // one of its part (due to wear and tear). If so, the inspector puts that robot
        // in a broken robots list and notifies the fixers.

        // Following messages are appropriate for this class
        // System.out.printf( "Robot %02d : Detected a broken robot (%02d), adding it to broken robots list.%n", ...);
        // System.out.printf( "Robot %02d : Notifying waiting fixers.%n", ...);

        try {

            Object currSerialNo = Common.get(robot, "serialNo");
            List<Robot> robots = SimulationRunner.factory.robots;
            boolean isEmpty = true;
            synchronized (robots) {

            for(Robot currRobot : robots) {
                    Object currRobotSerialNo = Common.get(currRobot, "serialNo");
                    Object arm = Common.get(currRobot, "arm");
                    Object logic = Common.get(currRobot, "logic");
                    Object payload = Common.get(currRobot, "payload");
                    if(arm == null || logic == null || payload == null) { // if either arm, logic or payload of a robot is broken
                        synchronized (SimulationRunner.factory.brokenRobots) {
                            if(!SimulationRunner.factory.brokenRobots.contains(currRobot)) { // if this robot is not already in brokenrobots list
                                SimulationRunner.factory.brokenRobots.add(currRobot); // add that robot to brokenrobots list
                                System.out.printf( "Robot %02d : Detected a broken robot (%02d), adding it to broken robots list.%n", currSerialNo, currRobotSerialNo);
                                isEmpty = false;
                            }
                        }
                    }
                }

            }

            if(!isEmpty) {
                System.out.printf( "Robot %02d : Notifying waiting fixers.%n", currSerialNo);

                synchronized (SimulationRunner.factory.brokenRobots) {
                    SimulationRunner.factory.brokenRobots.notifyAll();
                }
            }



            /*synchronized (SimulationRunner.factory.robots) {
                for(Robot r : SimulationRunner.factory.robots) {
                    Object fixerLogic = Common.get(r, "logic");
                    if(fixerLogic != null && Common.getClassName(fixerLogic.getClass().toString()).equals("Fixer")) {
                        r.notify(); // notify builders
                    }
                }
            }*/

        } catch (Exception e) {
            e.printStackTrace();
        }


    }
}