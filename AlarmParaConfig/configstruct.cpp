#include "configstruct.h"

void config_data::Clear()
{
    this->m_nconfig_id = -1;
    this->m_qmap_fields.clear();
    this->m_qmap_policy.clear();
    this->m_qstr_config_info.clear();
    this->m_qstr_text_font.clear();
    this->m_qstr_config_name.clear();
    return;
}

config_data &config_data::operator =(const config_data &dst)
{
    this->m_nconfig_id = dst.m_nconfig_id;
    this->m_qstr_config_name.operator =(dst.m_qstr_config_name);
    this->m_qstr_text_font.operator =(dst.m_qstr_text_font);
    this->m_qstr_config_info.operator =(dst.m_qstr_config_info);
    this->m_qmap_fields.operator =(dst.m_qmap_fields);
    this->m_qmap_policy.operator =(dst.m_qmap_policy);
    return *this;
}

AlarmColor &alarm_color::operator =(const alarm_color &dst)
{
    this->m_nalarm_level = dst.m_nalarm_level;
    this->m_qstr_alarm_name.operator =(dst.m_qstr_alarm_name);
    this->m_qstr_alarm_text.operator =(dst.m_qstr_alarm_text);
    this->m_qstr_alarm_back.operator =(dst.m_qstr_alarm_back);
    this->m_qstr_alarm_flash_text.operator =(dst.m_qstr_alarm_flash_text);
    this->m_qstr_alarm_flash_back.operator =(dst.m_qstr_alarm_flash_back);
    this->m_qstr_recover_text.operator =(dst.m_qstr_recover_text);
    this->m_qstr_recover_back.operator =(dst.m_qstr_recover_back);
    this->m_qstr_recover_flash_text.operator =(dst.m_qstr_recover_flash_text);
    this->m_qstr_recover_flash_back.operator =(dst.m_qstr_recover_flash_back);
    return *this;
}
