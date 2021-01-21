import java.io.IOException;

public class Main {
    public static void main(String[] args) throws IOException  {
        SIS informationSystem = new SIS();
        int sid = 5021430; // 1901834
        int c_code = 5710336; // 5710443
        double grade = 90.;
        int year = 20222; // 20192
        //double s = informationSystem.getGrade(sid, c_code, year);
        //System.out.println("Grade of " + sid + " for " +c_code + " in " + year + " is : " + s);
        //informationSystem.updateExam(sid, c_code, "Midterm2", grade);
        //double ys = informationSystem.getGrade(sid, c_code, year);
        //System.out.println("Grade of: " + sid + " for " + c_code + " in " + year + " is : " + ys);
        informationSystem.createTranscript(sid);
        //informationSystem.findCourse(c_code);
        //informationSystem.createHistogram(c_code, year);
    }
}