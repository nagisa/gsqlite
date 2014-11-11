#include "sqlview.hh"
#include <sqlitemm/sqlitemm.hh>

using namespace GSQLiteui;

SQLView::SQLView(std::shared_ptr<Connection> c)
    : Glib::ObjectBase("gsqlite_SQLView")
    , Gtk::Paned(Gtk::Orientation::ORIENTATION_VERTICAL)
    , c(c)
    , buffer(Gsv::Buffer::create())
    , sqlentry(this->buffer)
    , entry_box(Gtk::Orientation::ORIENTATION_VERTICAL)
{
    this->property_position() = 200;
    this->property_position_set() = true;

    Gsv::init();
    auto lman = Gsv::LanguageManager::get_default();
    auto lang = lman->get_language("sql");
    this->buffer->set_language(lang);
    this->sqlentry.set_show_line_numbers(true);
    this->sqlentry.set_highlight_current_line(true);
    this->sqlentry.set_insert_spaces_instead_of_tabs(true);
    this->sqlentry.set_indent_width(4);
    this->sqlentry.set_draw_spaces(Gsv::DrawSpacesFlags::DRAW_SPACES_TRAILING |
                                   Gsv::DrawSpacesFlags::DRAW_SPACES_TAB |
                                   Gsv::DrawSpacesFlags::DRAW_SPACES_NBSP |
                                   Gsv::DrawSpacesFlags::DRAW_SPACES_SPACE);
    this->buffer->property_has_selection().signal_changed().connect([this](){
        this->update_button();
    });

    this->entry_scroll.add(this->sqlentry);
    this->entry_box.pack_start(this->entry_scroll, true, true, 0);
    this->entry_box.pack_start(this->actions_bar, false, true, 0);
    this->actions_bar.pack_start(this->execute_query);
    this->update_button();
    this->pack1(this->entry_box, true, false);

    this->error_infobar.set_message_type(Gtk::MessageType::MESSAGE_ERROR);
    this->error_infobar.set_show_close_button(true);
    auto area = static_cast<Gtk::Container *>(
        this->error_infobar.get_content_area()
    );
    area->add(this->error_message);
    this->error_infobar.signal_response().connect([this](int rid){
        if(rid == Gtk::RESPONSE_CLOSE)
            this->close_infobar();
    });
    this->entry_box.pack_start(this->error_infobar, false, true, 0);
    this->error_infobar.set_no_show_all(true);
    this->pack2(this->results_scroll, true, false);
    this->results_scroll.set_no_show_all(true);

    this->execute_query.signal_clicked().connect([this](){
        this->exec_action();
    });
}

SQLView::~SQLView()
{
}

void
SQLView::update_button()
{
    auto sel = this->buffer->get_has_selection();
    this->execute_query.set_label(sel ? "Execute current selection"
                                      : "Execute current line");
}

void
SQLView::close_infobar()
{
    this->error_infobar.hide();
}

void
SQLView::exec_action()
{
    Glib::ustring query;
    if(this->buffer->get_has_selection()){
        auto sel = this->buffer->get_selection_bound();
        auto cur = this->buffer->get_insert();
        auto start = this->buffer->get_iter_at_mark(sel);
        auto end = this->buffer->get_iter_at_mark(cur);
        query = start.get_slice(end);
    } else {
        auto st = this->buffer->get_iter_at_mark(this->buffer->get_insert());
        auto en = st;
        if(!st.starts_line())
            st.set_line(st.get_line());
        if(!en.ends_line())
            en.forward_to_line_end();
        query = st.get_slice(en);
    }

    try {
        QueryResultsWidget::statement_t stmt(this->c->prepare(query.c_str()));
        auto widget = new QueryResultsWidget(std::move(stmt));
        this->results.reset(widget);
        if(this->results){
            this->results_scroll.remove();
        }
        this->results_scroll.add(*this->results);
        this->results_scroll.show();
        this->results->show_all();
        this->close_infobar();
    } catch (SQLiteError &e){
        this->results_scroll.hide();
        this->error_infobar.set_no_show_all(false);
        this->error_infobar.show_all();
        this->error_message.set_text(e.what());
    }
}
