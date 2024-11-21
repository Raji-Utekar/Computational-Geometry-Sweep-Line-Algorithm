#include <iostream>
#include <fstream> // For file handling
#include <vector>
#include <algorithm>
using namespace std;

//Point class having x and y coordinates
class Point
{
public:
    int x, y;
    Point()
    {
        x = 0;
        y = 0;
    }
    Point(int x, int y)
    {
        this->x = x;
        this->y = y;
    }
};

//Segment class with start and end point and Segment ID
class Segment
{
public:
    Point left, right;
    int id;
    Segment() {
        left = { 0, 0 };
        right = { 0, 0 };
        id = 0;
    }
    Segment(Point left, Point right, int id)
    {
        this->left = left;
        this->right = right;
        this->id = id;
    }
};

/* Event class: An event is a point (x, y) representing either the left or right endpoint of a segment. */
class Event
{
public:
    int x, y;
    bool isLeft;
    int SegID;

    Event() {
        x = 0;
        y = 0;
        isLeft = true;
        SegID = 0;
    }

    Event(int x, int y, bool isLeft, int segID)
    {
        this->x = x;
        this->y = y;
        this->isLeft = isLeft;
        this->SegID = segID;
    }

    bool operator<(const Event& other) const {
        if (x == other.x) return y < other.y;
        return x < other.x;
    }

};

// Function to find the orientation of ordered triplet (p, q, r)
int orientation(Point p, Point q, Point r)
{
    int slope = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
    if (slope == 0)
        return 0;
    return (slope > 0) ? 1 : 2;
}

// Function to check if a point q lies on segment pr
bool onSegment(Point p, Point q, Point r)
{
    return q.x <= max(p.x, r.x) && q.x >= min(p.x, r.x) &&
        q.y <= max(p.y, r.y) && q.y >= min(p.y, r.y);
}

// Function that returns true if two segments 's1' and 's2' intersect
bool doIntersect(Segment s1, Segment s2)
{
    Point p1 = s1.left, q1 = s1.right;
    Point p2 = s2.left, q2 = s2.right;

    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);

    if (o1 != o2 && o3 != o4)
        return true;

    if (o1 == 0 && onSegment(p1, p2, q1)) return true;
    if (o2 == 0 && onSegment(p1, q2, q1)) return true;
    if (o3 == 0 && onSegment(p2, p1, q2)) return true;
    if (o4 == 0 && onSegment(p2, q1, q2)) return true;

    return false;
}

// Function that checks for any intersection between segments
void findIntersections(vector<Segment>& segments)
{
    int n = segments.size();
    vector<Event> events;

    for (int i = 0; i < n; i++)
    {
        events.push_back(Event(segments[i].left.x, segments[i].left.y, true, i));
        events.push_back(Event(segments[i].right.x, segments[i].right.y, false, i));
    }

    sort(events.begin(), events.end());

    vector<Event> activeSegments;

    for (int i = 0; i < 2 * n; i++)
    {
        int curr_index = events[i].SegID;
        if (events[i].isLeft) {
            activeSegments.push_back(events[i]);
            if (activeSegments.size() > 1) {
                for (auto it = activeSegments.rbegin() + 1; it != activeSegments.rend(); ++it)
                {
                    if (doIntersect(segments[curr_index], segments[it->SegID]))
                    {
                        cout << "Line " << (it->SegID) + 1 << " " << curr_index + 1 << " intersects" << endl;
                    }
                }
            }
        }
        else
        {
            vector<Event> ::iterator iter; //make an iterator iter to search in activeSegments vector
            for (iter = activeSegments.begin(); iter != activeSegments.end(); iter++)
            {
                if (iter->SegID == curr_index) //if iter's segID == curr_index
                {
                    activeSegments.erase(iter); //then erase that element from activeSegments by passing iter to it
                    break; //and break it once found
                }
            }
        }
    }
}

int main()
{
    ifstream inFile("input.txt"); // Open the input file
    if (!inFile) {
        cerr << "Error opening file!" << endl;
        return 1;
    }

    int n;
    inFile >> n; // Read the number of segments from the file
    vector<Segment> segments;

    for (int i = 0; i < n; i++)
    {
        int x1, y1, x2, y2;
        inFile >> x1 >> y1 >> x2 >> y2; // Read each segment's coordinates from the file
        segments.push_back(Segment(Point(x1, y1), Point(x2, y2), i + 1));
    }

    inFile.close(); // Close the file

    // Find and print intersecting segments
    findIntersections(segments);

    return 0;
}
