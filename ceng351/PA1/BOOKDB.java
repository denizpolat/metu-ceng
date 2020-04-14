/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package ceng.ceng351.bookdb;


import java.sql.*;


/**
 *
 * @author e2237790
 */
public class BOOKDB implements IBOOKDB{

    private static String user = "2237790";//"pa1_user";
    private static String password = "735a97f8"; //"123456";
    private static String host = "144.122.71.65"; //"localhost";
    private static String database = "db2237790";//"pa1_recitation";
    private static int port = 8084; // 3306;

    private Connection con;


    public void initialize() {
        String url = "jdbc:mysql://" + host + ":" + port + "/" + database;

        try {
            Class.forName("com.mysql.cj.jdbc.Driver");
            con = DriverManager.getConnection(url, user, password);
        } catch (SQLException | ClassNotFoundException e) {
            e.printStackTrace();
        }
    }

    public int createTables() {

        int tablesInserted = 0;

        String createAuthor = "CREATE TABLE author(" +
                "author_id int," +
                "author_name varchar(60)," +
                "PRIMARY KEY (author_id))";

        String createPublisher = "CREATE TABLE publisher(" +
                "publisher_id int," +
                "publisher_name varchar(50)," +
                "PRIMARY KEY (publisher_id))";

        String createBook = "CREATE TABLE book(" +
                "isbn char(13)," +
                "book_name varchar(120)," +
                "publisher_id int," +
                "first_publish_year char(4)," +
                "page_count int," +
                "category varchar(25)," +
                "rating float," +
                "PRIMARY KEY (isbn)," +
                "FOREIGN KEY(publisher_id) REFERENCES publisher(publisher_id))";

        String createAuthOf = "CREATE TABLE author_of(" +
                "isbn char(13)," +
                "author_id int," +
                "PRIMARY KEY(isbn, author_id)," +
                "FOREIGN KEY (isbn) REFERENCES book(isbn)," +
                "FOREIGN KEY (author_id) REFERENCES author(author_id))";

        String createPhw1 = "CREATE TABLE phw1(" +
                "isbn char(13)," +
                "book_name varchar(120)," +
                "rating float," +
                "PRIMARY KEY (isbn))";


        //Author
        try {
            Statement auth = this.con.createStatement();
            auth.executeUpdate(createAuthor);
            tablesInserted++;
            auth.close();
        }
        catch(SQLException e) {
            e.printStackTrace();
        }


        //publisher
        try {
            Statement pub = this.con.createStatement();
            pub.executeUpdate(createPublisher);
            tablesInserted++;
            pub.close();
        }
        catch(SQLException e) {
            e.printStackTrace();
        }


        //book
        try {
            Statement book = this.con.createStatement();
            book.executeUpdate(createBook);
            tablesInserted++;
            book.close();
        }
        catch(SQLException e) {
            e.printStackTrace();
        }


        //author_of
        try {
            Statement authof = this.con.createStatement();
            authof.executeUpdate(createAuthOf);
            tablesInserted++;
            authof.close();
        }
        catch(SQLException e) {
            e.printStackTrace();
        }


        //phw1
        try {
            Statement phw1 = this.con.createStatement();
            phw1.executeUpdate(createPhw1);
            tablesInserted++;
            phw1.close();
        }
        catch(SQLException e) {
            e.printStackTrace();
        }
        return tablesInserted;
    }

    public int dropTables() {
        int sucDrop = 0;
        String dropAuthor = "DROP TABLE IF EXISTS author";
        String dropPublisher = "DROP TABLE IF EXISTS publisher";
        String dropBook = "DROP TABLE IF EXISTS book";
        String dropAuthOf = "DROP TABLE IF EXISTS author_of";
        String dropPhw1 = "DROP TABLE IF EXISTS phw1";


        try {
            Statement phw1 = this.con.createStatement();
            phw1.executeUpdate(dropPhw1);
            sucDrop++;
            phw1.close();

        } catch (SQLException e) {
            e.printStackTrace();
        }

        try {
            Statement author_of = this.con.createStatement();
            author_of.executeUpdate(dropAuthOf);
            sucDrop++;
            author_of.close();

        } catch (SQLException e) {
            e.printStackTrace();
        }

        try {
            Statement book = this.con.createStatement();
            book.executeUpdate(dropBook);
            sucDrop++;
            book.close();

        } catch (SQLException e) {
            e.printStackTrace();
        }

        try {
            Statement author = this.con.createStatement();
            author.executeUpdate(dropAuthor);
            sucDrop++;
            author.close();

        } catch (SQLException e) {
            e.printStackTrace();
        }


        try {
            Statement publisher = this.con.createStatement();
            publisher.executeUpdate(dropPublisher);
            sucDrop++;
            publisher.close();

        } catch (SQLException e) {
            e.printStackTrace();
        }

        return sucDrop;
    }


    public int insertAuthor(Author[] authors) {
        int suc = 0;
        int len = authors.length;
        for(int i=0; i<len; i++) {
            String auth = "INSERT INTO author VALUES(?,?)";
            try {
                PreparedStatement st = con.prepareStatement(auth);
                st.setInt(1, authors[i].getAuthor_id());
                st.setString(2, authors[i].getAuthor_name());
                st.executeUpdate();
                suc++;
                st.close();
            }
            catch (SQLException e) {
                e.printStackTrace();
            }
        }
        return suc;
    }

    public int insertBook(Book[] books) {
        int suc = 0;
        for(int i=0; i<books.length; i++) {
            String book = "INSERT INTO book VALUES(?,?,?,?,?,?,?)";
            try {
                PreparedStatement statement = con.prepareStatement(book);
                statement.setString(1, books[i].getIsbn());
                statement.setString(2, books[i].getBook_name());
                statement.setInt(3, books[i].getPublisher_id());
                statement.setString(4, books[i].getFirst_publish_year());
                statement.setInt(5, books[i].getPage_count());
                statement.setString(6, books[i].getCategory());
                statement.setDouble(7, books[i].getRating());
                statement.executeUpdate();
                suc++;
                statement.close();
            }
            catch (SQLException e) {
                e.printStackTrace();
            }
        }
        return suc;
    }
    public int insertPublisher(Publisher[] publishers){
        int suc = 0;
        int len = publishers.length;
        for(int i=0; i<len; i++) {
            String publisher = "INSERT INTO publisher VALUES(?,?)";
            try {
                PreparedStatement st = con.prepareStatement(publisher);
                st.setInt(1,publishers[i].getPublisher_id());
                st.setString(2, publishers[i].getPublisher_name());
                st.executeUpdate();
                suc++;
                st.close();
            } catch (SQLException e) {
                e.printStackTrace();
            }
        }
        return suc;
    }
    public int insertAuthor_of(Author_of[] author_ofs) {
        int suc = 0;
        int len = author_ofs.length;
        for(int i=0; i<len; i++) {
            String author_of = "INSERT INTO author_of VALUES (?, ?)";
            try {
                PreparedStatement st = con.prepareStatement(author_of);
                st.setString(1, author_ofs[i].getIsbn());
                st.setInt(2, author_ofs[i].getAuthor_id());
                st.executeUpdate();
                suc++;
                st.close();

            } catch (SQLException e) {
                e.printStackTrace();
            }
        }
        return suc;
    }

    public QueryResult.ResultQ1[] functionQ1() {
        ResultSet rs;
        QueryResult.ResultQ1[] res = {};
        String query = "SELECT b.isbn, b.first_publish_year, b.page_count, p.publisher_name " +
                        "FROM book b, publisher p " +
                        "WHERE p.publisher_id = b.publisher_id AND b.page_count = " +
                        "(SELECT MAX(b1.page_count) FROM book b1) " +
                        "ORDER BY b.isbn ASC";
        try {
            Statement s = con.createStatement();
            rs = s.executeQuery(query);
            rs.last();
            int len = rs.getRow();
            rs.first();
            res = new QueryResult.ResultQ1[len];
            int i = 0;
              while (i < len) {
                String isbn = rs.getString("isbn");
                String pub_year = rs.getString("first_publish_year");
                int page = rs.getInt("page_count");
                String pub_name = rs.getString("publisher_name");
                QueryResult.ResultQ1 a = new QueryResult.ResultQ1(isbn, pub_year, page, pub_name);
                res[i] = a;
                i++;
                rs.next();
            }
            s.close();

        }
        catch (Exception e) {
            e.printStackTrace();
        }
        return res;

    }

    public QueryResult.ResultQ2[] functionQ2(int author_id1, int author_id2) {
        ResultSet rs;
        QueryResult.ResultQ2[] res = {};
        String query = "SELECT b.publisher_id, AVG(b.page_count) as avg " +
                "FROM book b " +
                "WHERE b.publisher_id IN " +
                "(SELECT b1.publisher_id" +
                " FROM book b1, author_of ao1, author_of ao2 " +
                " WHERE ao1.author_id =" + author_id1 + " AND ao2.author_id = " + author_id2 +
                " AND ao1.isbn = ao2.isbn AND b1.isbn = ao1.isbn)" +
                " GROUP BY b.publisher_id" +
                " ORDER BY b.publisher_id ASC";
        try {
            Statement s = con.createStatement();
            rs = s.executeQuery(query);
            rs.last();
            int len = rs.getRow();
            rs.first();
            res = new QueryResult.ResultQ2[len];
            int i = 0;
            while (i < len){
                int pub_id = rs.getInt("publisher_id");
                double avg = rs.getDouble("avg");

                QueryResult.ResultQ2 a = new QueryResult.ResultQ2(pub_id, avg);
                res[i] = a;
                i++;
                rs.next();
            }
            s.close();

        }
        catch (Exception e) {
            e.printStackTrace();
        }
        return res;
    }

    public QueryResult.ResultQ3[] functionQ3(String author_name) {
        ResultSet rs;
        QueryResult.ResultQ3[] res = {};
        String query = "SELECT b.book_name, b.category, b.first_publish_year " +
                "FROM book b WHERE b.first_publish_year = " +
                "(SELECT MIN(b1.first_publish_year) " +
                "FROM book b1, author_of ao, author a " +
                "WHERE a.author_name = '" + author_name +
                "' AND ao.isbn = b1.isbn AND a.author_id = ao.author_id AND " +
                "b1.isbn = b.isbn) " +
                "ORDER BY b.book_name, b.category, b.first_publish_year ASC";
        try {
            Statement s = con.createStatement();
            rs = s.executeQuery(query);
            rs.last();
            int len = rs.getRow();
            rs.first();
            res = new QueryResult.ResultQ3[len];
            int i = 0;
            while (i < len){
                String name = rs.getString("book_name");
                String cat = rs.getString("category");
                String year = rs.getString("first_publish_year");
                QueryResult.ResultQ3 a = new QueryResult.ResultQ3(name, cat, year);
                res[i] = a;
                i++;
                rs.next();
            }
            s.close();

        }
        catch (Exception e) {
            e.printStackTrace();
        }
        return res;
    }

    public QueryResult.ResultQ4[] functionQ4() {
        ResultSet rs;
        QueryResult.ResultQ4[] res = {};
        String query = "SELECT DISTINCT b.publisher_id, b.category " +
                "FROM publisher p, book b, book b1, book b2, book b3 " +
                "WHERE p.publisher_name LIKE '% % %'  AND " +
                "b.publisher_id = p.publisher_id AND b1.isbn <> b2.isbn AND " +
                "b2.isbn <> b3.isbn AND b1.isbn <> b3.isbn AND b1.publisher_id = p.publisher_id AND " +
                "b2.publisher_id = p.publisher_id AND b3.publisher_id = p.publisher_id " +
                "AND  3 < (SELECT AVG(bk.rating)" +
                "FROM book bk WHERE bk.publisher_id = b.publisher_id) " +
                "ORDER BY b.publisher_id, b.category ASC";
        try {
            Statement s = con.createStatement();
            rs = s.executeQuery(query);
            rs.last();
            int len = rs.getRow();
            rs.first();
            res = new QueryResult.ResultQ4[len];
            int i = 0;
            while (i < len) {
                int pid = rs.getInt("publisher_id");
                String cat = rs.getString("category");
                QueryResult.ResultQ4 a = new QueryResult.ResultQ4(pid, cat);
                res[i] = a;
                i++;
                rs.next();
            }
            s.close();

        }
        catch (Exception e) {
            e.printStackTrace();
        }
        return res;
    }

    public QueryResult.ResultQ5[] functionQ5(int author_id) {
        ResultSet rs;
        QueryResult.ResultQ5[] res = {};
        String query = "SELECT a.author_id, a.author_name " +
                "FROM author a, author_of ao " +
                "WHERE a.author_id = ao.author_id AND NOT EXISTS ( " +
                "SELECT * FROM author_of a2, book b2 " +
                "WHERE a2.isbn = b2.isbn AND a2.author_id = " + author_id + " AND NOT EXISTS (" +
                "SELECT * FROM book b WHERE b.isbn = ao.isbn AND b.publisher_id = b2.publisher_id ))" +
                "ORDER BY a.author_id ASC";
        try {
            Statement s = con.createStatement();
            rs = s.executeQuery(query);
            rs.last();
            int len = rs.getRow();
            rs.first();
            res = new QueryResult.ResultQ5[len];
            int i = 0;
            while (i < len) {
                int aid = rs.getInt("author_id");
                String name = rs.getString("author_name");
                QueryResult.ResultQ5 a = new QueryResult.ResultQ5(aid, name);
                res[i] = a;
                i++;
                rs.next();
            }
            s.close();

        }
        catch (Exception e) {
            e.printStackTrace();
        }
        return res;
    }

    public QueryResult.ResultQ6[] functionQ6() {
        ResultSet rs;
        QueryResult.ResultQ6[] res = {};
        String query = "SELECT ao.author_id, b.isbn " +
                "FROM author_of ao, book b " +
                "WHERE ao.isbn = b.isbn AND ao.author_id NOT IN (" +
                "SELECT DISTINCT ao2.author_id " +
                "FROM author_of ao2, author_of ao3, book b2, book b3 " +
                "WHERE ao2.author_id <> ao3.author_id AND ao3.isbn = b3.isbn AND " +
                "ao2.isbn = b2.isbn AND b2.publisher_id = b3.publisher_id) " +
                "ORDER BY ao.author_id, b.isbn ASC";
        try {
            Statement s = con.createStatement();
            rs = s.executeQuery(query);
            rs.last();
            int len = rs.getRow();
            rs.first();
            res = new QueryResult.ResultQ6[len];
            int i = 0;
            while (i < len) {
                int aid = rs.getInt("author_id");
                String isbn = rs.getString("isbn");
                QueryResult.ResultQ6 a = new QueryResult.ResultQ6(aid, isbn);
                res[i] = a;
                i++;
                rs.next();
            }
            s.close();

        }
        catch (Exception e) {
            e.printStackTrace();
        }
        return res;
    }

    public QueryResult.ResultQ7[] functionQ7(double rating) {
        ResultSet rs;
        QueryResult.ResultQ7[] res = {};
        String query = "SELECT p.publisher_id, p.publisher_name " +
                "FROM publisher p, book b " +
                "WHERE b.publisher_id = p.publisher_id AND p.publisher_id IN (" +
                "SELECT DISTINCT p1.publisher_id " +
                "FROM publisher p1, book b1, book b2 " +
                "WHERE b1.publisher_id = p.publisher_id AND b2.publisher_id = p.publisher_id AND " +
                "b1.isbn <> b2.isbn AND b1.category = 'Roman' AND b2.category = 'Roman') " +
                "GROUP BY p.publisher_id " +
                "HAVING AVG(b.rating) >" + rating +
                " ORDER BY p.publisher_id ASC";
        try {
            Statement s = con.createStatement();
            rs = s.executeQuery(query);
            rs.last();
            int len = rs.getRow();
            rs.first();
            res = new QueryResult.ResultQ7[len];
            int i = 0;
            while (i < len) {
                int pid = rs.getInt("publisher_id");
                String name = rs.getString("publisher_name");
                QueryResult.ResultQ7 a = new QueryResult.ResultQ7(pid, name);
                res[i] = a;
                i++;
                rs.next();
            }
            s.close();

        }
        catch (Exception e) {
            e.printStackTrace();
        }
        return res;
    }




    public QueryResult.ResultQ8[] functionQ8() {
        ResultSet rs;
        QueryResult.ResultQ8[] res = {};
        String query = "SELECT b.isbn, b.book_name, b.rating " +
                "FROM book b, book b2 " +
                "WHERE b.book_name = b2.book_name AND b.isbn <> b2.isbn " +
                "AND b.rating = (" +
                "SELECT MIN(b3.rating) " +
                "FROM book b3 " +
                "WHERE b3.book_name = b.book_name) " +
                " ORDER BY b.isbn ASC";
        try {
            Statement s = con.createStatement();
            rs = s.executeQuery(query);
            rs.last();
            int len = rs.getRow();
            rs.first();
            res = new QueryResult.ResultQ8[len];
            int i = 0;
            while (i < len) {
                String isbn = rs.getString("isbn");
                double rating = rs.getDouble("rating");
                String name = rs.getString("book_name");
                String phw1 = "INSERT INTO phw1 VALUES(?,?,?)";
                try {
                    PreparedStatement st = con.prepareStatement(phw1);
                    st.setString(1, isbn);
                    st.setString(2, name);
                    st.setDouble(3, rating);
                    st.executeUpdate();
                    st.close();
                }
                catch (SQLException e) {
                    e.printStackTrace();
                }
                QueryResult.ResultQ8 a = new QueryResult.ResultQ8(isbn, name, rating);
                res[i] = a;
                i++;
                rs.next();
            }
            s.close();

        }
        catch (Exception e) {
            e.printStackTrace();
        }
        return res;
    }

    public double functionQ9(String keyword) {
        ResultSet rs;
        double sum = 0;
        String query = "UPDATE book " +
                "SET rating = rating + 1 " +
                "WHERE book_name LIKE '%" + keyword + "%' AND rating <= 4 ";
        String query2 = "SELECT SUM(b.rating) as sm FROM book b";
        try {
            PreparedStatement s = con.prepareStatement(query);
            s.executeUpdate();
            s.close();
        }
        catch (Exception e) {
            e.printStackTrace();
        }

        try {
            Statement st = con.createStatement();
            rs = st.executeQuery(query2);
            rs.first();
            sum = rs.getInt("sm");
        }
        catch (SQLException e) {
            e.printStackTrace();
        }
        return sum;
    }

    public int function10() {
        ResultSet rs;
        int count = 0;
        String query = "DELETE FROM publisher " +
                "WHERE publisher_id NOT IN (SELECT b.publisher_id FROM book b)";
        String query2 = "SELECT COUNT(*) as cnt FROM publisher ";
        try {
            PreparedStatement s = con.prepareStatement(query);
            s.executeUpdate();
            s.close();
        }
        catch (Exception e) {
            e.printStackTrace();
        }

        try {
            Statement st = con.createStatement();
            rs = st.executeQuery(query2);
            rs.first();
            count = rs.getInt("cnt");
        }
        catch (SQLException e) {
            e.printStackTrace();
        }
        return count;
    }

}