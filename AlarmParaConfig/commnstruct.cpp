#include "commnstruct.h"
void AlarmConfig::Clear()
{
    this->m_nnodekey = -1;
    this->m_qstr_nodename.clear();
    this->m_map_field.clear();
    this->m_map_policy.clear();
    this->m_map_color.clear();
    this->m_qstr_font.clear();
    this->m_qstr_info.clear();
    return;
}

AlarmConfig &AlarmConfig::operator =(const AlarmConfig &dst)
{
    this->Clear();
    this->m_nnodekey = dst.m_nnodekey;
    this->m_qstr_nodename = dst.m_qstr_nodename;
    this->m_map_field = dst.m_map_field;
    this->m_map_policy = dst.m_map_policy;
    this->m_map_color = dst.m_map_color;
    this->m_qstr_font = dst.m_qstr_font;
    this->m_qstr_info = dst.m_qstr_info;
    return *this;
}

void AlarmColour::Clear()
{
    this->m_nlevel = -1;
    this->m_qstr_levelname.clear();
    this->m_qstr_alarm_text.clear();
    this->m_qstr_alarm_background.clear();
    this->m_qstr_alarm_flash_text.clear();
    this->m_qstr_alarm_flash_background.clear();
    this->m_qstr_recover_text.clear();
    this->m_qstr_recover_background.clear();
    this->m_qstr_recover_flash_text.clear();
    this->m_qstr_recover_flash_background.clear();

    this->m_qstr_alarm_text = "#FF0000";
    this->m_qstr_alarm_background = "#000000";
    this->m_qstr_alarm_flash_text = "#2F705F";
    this->m_qstr_alarm_flash_background = "#000000";
    this->m_qstr_recover_text = "#00FF00";
    this->m_qstr_recover_background = "#000000";
    this->m_qstr_recover_flash_text = "#0000FF";
    this->m_qstr_recover_flash_background = "#000000";

    return;
}

AlarmColour &AlarmColour::operator =(const AlarmColour &dst)
{
    this->Clear();
    this->m_nlevel = dst.m_nlevel;
    this->m_qstr_levelname = dst.m_qstr_levelname;
    this->m_qstr_alarm_text = dst.m_qstr_alarm_text;
    this->m_qstr_alarm_background = dst.m_qstr_alarm_background;
    this->m_qstr_alarm_flash_text = dst.m_qstr_alarm_flash_text;
    this->m_qstr_alarm_flash_background = dst.m_qstr_alarm_flash_background;
    this->m_qstr_recover_text = dst.m_qstr_recover_text;
    this->m_qstr_recover_background = dst.m_qstr_recover_background;
    this->m_qstr_recover_flash_text = dst.m_qstr_recover_flash_text;
    this->m_qstr_recover_flash_background = dst.m_qstr_recover_flash_background;
    return *this;
}
