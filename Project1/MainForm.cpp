#include "MainForm.h"

#define CSV_DELIMETER ";"
#define CSV_DELIMETER_CHR ';'

System::Void Project1::MainForm::MainForm_Load(System::Object^ sender, System::EventArgs^ e)
{
    CreateDistrictTab("Краснокутский р-н");
    CreateDistrictTab("Дергачевский р-н");
    CreateDistrictTab("Балтайский р-н");
    CreateDistrictTab("Ал. Гайский р-н");

    return System::Void();
}

void Project1::MainForm::CreateDistrictTab(String^ name)
{
    System::Windows::Forms::TabPage^ district = (gcnew System::Windows::Forms::TabPage());
    district->Location = System::Drawing::Point(4, 25);
    district->Name = L"disctrict";
    district->Text = name;
    district->UseVisualStyleBackColor = true;

    System::Windows::Forms::DataGridView^ grid = (gcnew System::Windows::Forms::DataGridView());
    district->Controls->Add(grid);
    grid->AllowUserToAddRows = false;
    grid->AllowUserToDeleteRows = false;
    grid->AllowUserToOrderColumns = false;
    grid->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
    grid->Dock = System::Windows::Forms::DockStyle::Fill;
    grid->Name = L"grid";
    grid->RowHeadersWidth = 51;
    grid->RowTemplate->Height = 24;
    grid->TabIndex = 0;

    LoadDataFromTemplate(grid);

    this->disctrictTabControl->Controls->Add(district);
}

System::Void Project1::MainForm::сохранитьToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
{
    SaveFileDialog^ saveFileDialog = gcnew SaveFileDialog();
    saveFileDialog->Filter = "(csv file(*.csv)| *.csv";
    try
    {
        if (saveFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK)
        {
            String^ path = saveFileDialog->FileName;

            TabPage^ tabPage = this->disctrictTabControl->SelectedTab;
            DataGridView^ grid = safe_cast<DataGridView^>(tabPage->Controls[0]);
            String^ collectedData = GetCSVData(grid);

            System::IO::File::WriteAllText(path, collectedData, System::Text::Encoding::UTF8);

            MessageBox::Show("Таблица \"" + tabPage->Text + "\" успешно сохранена по пути\n" + path);
        }
        else {
            return System::Void();
        }
    }
    catch (Exception^ ex) {
        MessageBox::Show(this, "Не удалось сохранить. Возможно файл открыт в другой программе.", "Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
    }

    return System::Void();
}

System::String^ Project1::MainForm::GetCSVData(DataGridView^ grid)
{    
    System::Text::StringBuilder^ sb = gcnew System::Text::StringBuilder();

    sb->Append(grid->TopLeftHeaderCell->Value->ToString());
    sb->Append(CSV_DELIMETER);
    for (int i = 0; i < grid->Columns->Count; i++) {
        System::Object^ header = grid->Columns[i]->HeaderCell->Value;
        if (header == nullptr) header = "";
        sb->Append(header->ToString());
        sb->Append(CSV_DELIMETER);
    }
    sb->Append(CSV_DELIMETER);
    sb->Append("\n");

    for (int i = 0; i < grid->Rows->Count; i++) {
        System::Object^ header = grid->Rows[i]->HeaderCell->Value;
        if (header == nullptr) header = "";
        sb->Append(header->ToString());
        sb->Append(CSV_DELIMETER);

        DataGridViewRow^ row = grid->Rows[i];
        for (int j = 0; j < row->Cells->Count; j++) {
            System::Object^ val = row->Cells[j]->Value;
            if (val == nullptr) val = "";
            sb->Append(val);
            sb->Append(CSV_DELIMETER);
        }
        sb->Append("\n");
    }

    return sb->ToString();
}

void Project1::MainForm::LoadDataFromTemplate(System::Windows::Forms::DataGridView^ grid)
{
    array<String^>^ rows = System::IO::File::ReadAllLines("template.csv", System::Text::Encoding::UTF8);
    array<String^>^ row0 = rows[0]->Split(CSV_DELIMETER_CHR);
    grid->TopLeftHeaderCell->Value = row0[0];
    for (int i = 1; i < row0->Length; i++) {
        grid->Columns->Add(row0[i], row0[i]);
        grid->Columns[i - 1]->SortMode = DataGridViewColumnSortMode::NotSortable;
    }
    grid->Rows->Add(rows->Length - 1);
    for (int i = 1; i < rows->Length; i++) {
        array<String^>^ cells = rows[i]->Split(CSV_DELIMETER_CHR);

        grid->Rows[i - 1]->HeaderCell->Value = cells[0];
        grid->Rows[i - 1]->HeaderCell->Style->BackColor = System::Drawing::Color::LightGray;

        for (int j = 1; j < cells->Length; j++) {
            DataGridViewCell^ cell = grid->Rows[i - 1]->Cells[j - 1];
            if (!cells[j]->Equals("<EDIT>"))
            {
                cell->Value = cells[j];
                cell->ReadOnly = true;
                cell->Style->BackColor = System::Drawing::Color::LightGray;
            }
        }
    }
}