import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.*;
import java.util.stream.Collectors;
import java.util.stream.IntStream;
import java.util.stream.Stream;

public class SIS {
    private static String fileSep = File.separator;
    private static String lineSep = System.lineSeparator();
    private static String space   = " ";

    private List<Student> studentList = new ArrayList<>();

    public SIS(){ processOptics(); }



    private void processOptics() {
        // TODO
        try {
            Stream<Path> paths = Files.list(Paths.get("input"));
            paths.forEach(student -> {
                try {
                    Stream<String> lines = Files.lines(student);
                    List<String> linesList = lines.collect(Collectors.toList());
                    String line = linesList.get(0);
                    int spaceIndex = line.lastIndexOf(space);
                    String name = line.substring(0, spaceIndex);
                    int stdID = Integer.parseInt(line.substring(spaceIndex+1));
                    spaceIndex = name.lastIndexOf(space);
                    String lastName = name.substring(spaceIndex+1);
                    String[] names = name.substring(0, spaceIndex).split(space);
                    Student thisStudent = null;
                    for(Student std : studentList) {
                        if(std.getStudentID() == stdID) thisStudent = std;
                    }
                    if(thisStudent == null) {
                        thisStudent = new Student(names, lastName, stdID);
                        studentList.add(thisStudent);
                    }

                    line = linesList.get(1);
                    String[] secondLineItems = line.split(space); // year coursecode credit
                    int year = Integer.parseInt(secondLineItems[0]);
                    int courseCode = Integer.parseInt(secondLineItems[1]);
                    int credit = Integer.parseInt(secondLineItems[2]);
                    String examType = linesList.get(2);
                    line = linesList.get(3);
                    //System.out.println(examType);
                    double grade = 100. / line.length();
                    // System.out.println(grade);
                    double correctAnswerCount = line.chars().filter(ch -> ch == 'T').count();
                    //System.out.println(correctAnswerCount);
                    grade *= correctAnswerCount;
                    // if(grade <60 && courseCode == 6390102) System.out.println("Gradeeeeeee  " + grade + "   year " + year + " stdID " + stdID + " examtypeee " + examType);
                    Course course = new Course(courseCode, year, examType, credit, grade);
                    thisStudent.getTakenCourses().add(course);
                    //lines.map(line -> line.split(space)).;
                } catch (IOException e) {
                    e.printStackTrace();
                }
            });
        } catch (IOException ioe) {
            ioe.printStackTrace();
        }
    }

    public double getGrade(int studentID, int courseCode, int year){
        // TODO
        List<Course> courseList = this.studentList.stream().filter(std -> std.getStudentID() == studentID)
                .map(course -> course.getTakenCourses().stream().filter(crs -> crs.getCourseCode() == courseCode)
                        .filter(crs -> crs.getYear() == year).collect(Collectors.toList())).findAny().orElse(null);

        if(courseList == null) return 0.;
        double mt1 = courseList.stream().filter(c -> c.getExamType().equals("Midterm1")).map(Course::getGrade).findAny().orElse(0.);
        double mt2 = courseList.stream().filter(c -> c.getExamType().equals("Midterm2")).map(Course::getGrade).findAny().orElse(0.);
        double final_ = courseList.stream().filter(c -> c.getExamType().equals("Final")).map(Course::getGrade).findAny().orElse(0.);
        return mt1 / 4. + mt2 / 4. + final_ / 2.;
    }

    public void updateExam(int studentID, int courseCode, String examType, double newGrade){
        // TODO
        this.studentList.stream().filter(std -> std.getStudentID() == studentID)
                .map(course -> course.getTakenCourses().stream().filter(crs -> crs.getCourseCode() == courseCode)
                        .filter(crs -> crs.getExamType().equals(examType))
                        .max(Comparator.comparingInt(Course::getYear))).findAny().get() // inside max : (crs1, crs2) -> (crs1.getYear() - crs2.getYear()) -- instead of findAny : .collect(Collectors.toList()).get(0)
                .orElse(null).setGrade(newGrade);


    }

    public void createTranscript(int studentID){
        // TODO

        Map<String, Double> letterToWeight = new HashMap<>();
        letterToWeight.put("AA", 4.);
        letterToWeight.put("BA", 3.5);
        letterToWeight.put("BB", 3.);
        letterToWeight.put("CB", 2.5);
        letterToWeight.put("CC", 2.);
        letterToWeight.put("DC", 1.5);
        letterToWeight.put("DD", 1.);
        letterToWeight.put("FD", 0.5);

        Map<Double, String> gradeToLetter = new HashMap<>();
        gradeToLetter.put(19., "AA");
        gradeToLetter.put(18., "AA");
        gradeToLetter.put(17., "BA");
        gradeToLetter.put(16., "BB");
        gradeToLetter.put(15., "CB");
        gradeToLetter.put(14., "CC");
        gradeToLetter.put(13., "DC");
        gradeToLetter.put(12., "DD");
        gradeToLetter.put(11., "FD");
        gradeToLetter.put(10., "FD");


        List<Course> coursesOfStd = this.studentList.stream().filter(std -> std.getStudentID() == studentID)
                .flatMap(std -> std.getTakenCourses().stream().sorted(Comparator.comparingInt(Course::getYear).reversed()))
                .collect(Collectors.toList());

        Collection<Optional<Course>> uniqueCourses = coursesOfStd.stream().collect(Collectors.groupingBy(Course::getCourseCode, Collectors.maxBy(Comparator.comparingInt(Course::getYear)))).values();

        double totalGrade = uniqueCourses.stream().mapToDouble(c -> c.get().getCredit() * letterToWeight.getOrDefault(gradeToLetter.getOrDefault(Math.floor(Math.round(getGrade(studentID, c.get().getCourseCode(), c.get().getYear())) / 5.), "FF"), 0.)).sum();
        int totalCredit = uniqueCourses.stream().mapToInt(c -> c.get().getCredit()).sum();

        Map<Integer, List<Course>> gradesMap = coursesOfStd.stream().collect(Collectors.groupingBy(Course::getYear, TreeMap::new, Collectors.toList()));
                gradesMap.forEach((year, courses) -> {
                    System.out.println(year);
                    courses.stream().sorted(Comparator.comparingInt(Course::getCourseCode)).map(Course::getCourseCode).distinct().forEach(crs ->
                        System.out.println(crs + " " + gradeToLetter.getOrDefault(Math.floor(Math.round(getGrade(studentID, crs, year)) / 5.), "FF")));
                });
        System.out.printf(Locale.US, "CGPA: %.2f%n",totalGrade/totalCredit);
        //System.out.println("CGPA: " + totalGrade/totalCredit);
    }

    public void findCourse(int courseCode){
        // TODO

        Map<Integer, Long> resultMap = this.studentList.stream().flatMap(std -> std.getTakenCourses().stream())
                .filter(c->c.getCourseCode() == courseCode).sorted(Comparator.comparingInt(Course::getYear)).collect(Collectors.groupingBy(Course::getYear,
                        TreeMap::new, Collectors.counting()));
        resultMap.forEach((year, count) -> {System.out.println(year + " " + count / 3);
        });
    }

    public void createHistogram(int courseCode, int year){
        // TODO

        Map<Double, Long> grades = this.studentList.stream().filter(s -> s.getTakenCourses().stream()
                .anyMatch(c -> c.getCourseCode() == courseCode && c.getYear() == year)).map(Student::getStudentID)
                .distinct().map(s -> getGrade(s, courseCode, year)).collect(Collectors.groupingBy(e->Math.floor(e/10.0), Collectors.counting()));

        // if there exists a grade = 100 so a group with key 10 is created, add this value to grades.get(9)
        grades.put(9., grades.getOrDefault(9., 0L) + grades.getOrDefault(10., 0L));
        
        IntStream.range(0, 10).forEach(key -> System.out.println(key*10 + "-" + (key+1)*10 + " " + grades.getOrDefault((double) key, 0L)));


//         Map<Double, Long> hist = this.studentList.stream().flatMap(std -> std.getTakenCourses().stream())
//                .filter(course -> course.getCourseCode() == courseCode).filter(course -> course.getYear() == year).filter(crs -> crs.getExamType().equals("Final"))
//                .collect(Collectors.groupingBy(e->Math.floor(e.getGrade()/10.0), Collectors.counting()));
//         IntStream.range(0,10).forEach(key -> System.out.println(key*10 + "-" + (key+1)*10 + " " + hist.getOrDefault((double) key, 0L)));

//        System.out.println("----------------------------------------------------");
//
//        List<Course> myCourse = this.studentList.stream().flatMap(std -> std.getTakenCourses().stream())
//                .filter(course -> course.getCourseCode() == courseCode).filter(course -> course.getYear() == year).collect(Collectors.toList());
//
//        System.out.println("0-10 " + myCourse.stream().filter(c -> c.getGrade() < 10 && c.getGrade() >= 0).count());
//        System.out.println("10-20 " + myCourse.stream().filter(c -> c.getGrade() < 20 && c.getGrade() >= 10).count());
//        System.out.println("20-30 " + myCourse.stream().filter(c -> c.getGrade() < 30 && c.getGrade() >= 20).count());
//        System.out.println("30-40 " + myCourse.stream().filter(c -> c.getGrade() < 40 && c.getGrade() >= 30).count());
//        System.out.println("40-50 " + myCourse.stream().filter(c -> c.getGrade() < 50 && c.getGrade() >= 40).count());
//        System.out.println("50-60 " + myCourse.stream().filter(c -> c.getGrade() < 60 && c.getGrade() >= 50).count());
//        System.out.println("60-70 " + myCourse.stream().filter(c -> c.getGrade() < 70 && c.getGrade() >= 60).count());
//        System.out.println("70-80 " + myCourse.stream().filter(c -> c.getGrade() < 80 && c.getGrade() >= 70).count());
//        System.out.println("80-90 " + myCourse.stream().filter(c -> c.getGrade() < 90 && c.getGrade() >= 80).count());
//        System.out.println("90-100 " + myCourse.stream().filter(c -> c.getGrade() < 100 && c.getGrade() >= 90).count());
    }
}
