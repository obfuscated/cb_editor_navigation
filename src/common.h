#ifndef _EDITOR_NAVIGATION_COMMON_H_
#define _EDITOR_NAVIGATION_COMMON_H_

#include <vector>

class cbProject;

class EditorPosition
{
public:
    EditorPosition() :
        m_project(NULL),
        m_line(-1),
        m_pos_in_line(-1)
    {
    }
    EditorPosition(cbProject *project, wxString const &filename, int line, int pos_in_line, int pos) :
        m_project(project),
        m_filename(filename),
        m_line(line),
        m_pos_in_line(pos_in_line),
        m_pos(pos)
    {
    }

    wxString const & GetFilename() const { return m_filename; }
    int GetLine() const { return m_line; }
    int GetPosInLine() const { return m_pos_in_line; }
    int GetPos() const { return m_pos; }
    cbProject const* GetProject() const { return m_project; }

    bool operator ==(EditorPosition const &p) const
    {
        return m_pos == p.m_pos && m_filename == p.m_filename && m_project == p.m_project;
    }
    bool operator !=(EditorPosition const &p) const
    {
        return !(*this == p);
    }
private:
    cbProject const *m_project;
    wxString m_filename;
    int m_line;
    int m_pos_in_line;
    int m_pos;
};
typedef std::vector<EditorPosition> EditorPositions;


#endif // _EDITOR_NAVIGATION_COMMON_H_
