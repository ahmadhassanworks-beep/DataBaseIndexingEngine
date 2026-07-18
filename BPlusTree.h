#ifndef BPLUSTREE_H
#define BPLUSTREE_H

#include <QString>
#include <QVector>
#include <QFile>
#include <QTextStream>
#include <QDir>

const int ORDER = 5;
const int MAX_RECORDS = 150;

struct Record {
    int key;
    QVector<QString> values;
};

struct BPlusNode {
    bool isLeaf;
    int numKeys;
    int keys[ORDER];
    Record records[ORDER];
    BPlusNode* children[ORDER + 1];
    BPlusNode* next;

    BPlusNode(bool leaf) {
        isLeaf = leaf;
        numKeys = 0;
        next = nullptr;
        for (int i = 0; i <= ORDER; i++)
            children[i] = nullptr;
    }
};

class BPlusTree {
public:
    BPlusNode* root;
    int totalRecords;

    BPlusTree() {
        root = nullptr;
        totalRecords = 0;
    }

    bool isFull() {
        return totalRecords >= MAX_RECORDS;
    }

    bool isDuplicate(int key) {
        if (!root) return false;
        BPlusNode* curr = root;
        while (!curr->isLeaf) {
            int i = 0;
            while (i < curr->numKeys && key >= curr->keys[i]) i++;
            curr = curr->children[i];
        }
        for (int i = 0; i < curr->numKeys; i++)
            if (curr->records[i].key == key) return true;
        return false;
    }

    bool insert(int key, QVector<QString> values) {
        if (isFull()) return false;
        if (isDuplicate(key)) return false;

        Record r;
        r.key = key;
        r.values = values;

        if (!root) {
            root = new BPlusNode(true);
            root->records[0] = r;
            root->keys[0] = key;
            root->numKeys = 1;
            totalRecords++;
            return true;
        }

        if (root->numKeys == ORDER) {
            BPlusNode* newRoot = new BPlusNode(false);
            newRoot->children[0] = root;
            splitChild(newRoot, 0, root);
            root = newRoot;
        }

        insertNonFull(root, r);
        totalRecords++;
        return true;
    }

    Record* search(int key) {
        if (!root) return nullptr;
        BPlusNode* curr = root;
        while (!curr->isLeaf) {
            int i = 0;
            while (i < curr->numKeys && key >= curr->keys[i]) i++;
            curr = curr->children[i];
        }
        for (int i = 0; i < curr->numKeys; i++)
            if (curr->records[i].key == key)
                return &curr->records[i];
        return nullptr;
    }

    bool update(int key, QVector<QString> newValues) {
        Record* r = search(key);
        if (!r) return false;
        r->values = newValues;
        return true;
    }

    bool remove(int key) {
        if (!root) return false;
        bool found = removeFromNode(root, key);
        if (found) totalRecords--;
        return found;
    }

    void getAllRecords(QVector<Record>& result) {
        if (!root) return;
        BPlusNode* curr = root;
        while (!curr->isLeaf)
            curr = curr->children[0];
        while (curr) {
            for (int i = 0; i < curr->numKeys; i++)
                result.append(curr->records[i]);
            curr = curr->next;
        }
    }

    void saveToFile(const QString& path, const QVector<QString>& columns) {
        QFile file(path);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return;
        QTextStream out(&file);
        out << "COLUMNS:";
        for (int i = 0; i < columns.size(); i++) {
            out << columns[i];
            if (i < columns.size() - 1) out << ",";
        }
        out << "\n";
        QVector<Record> all;
        getAllRecords(all);
        for (auto& r : all) {
            out << r.key;
            for (auto& v : r.values)
                out << "," << v;
            out << "\n";
        }
        file.close();
    }

    bool loadFromFile(const QString& path, QVector<QString>& columns) {
        QFile file(path);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return false;
        QTextStream in(&file);
        QString firstLine = in.readLine();
        if (!firstLine.startsWith("COLUMNS:")) return false;
        QString colPart = firstLine.mid(8);
        columns = colPart.split(",");
        while (!in.atEnd()) {
            QString line = in.readLine();
            if (line.trimmed().isEmpty()) continue;
            QStringList parts = line.split(",");
            if (parts.size() < 2) continue;
            int key = parts[0].toInt();
            QVector<QString> values;
            for (int i = 1; i < parts.size(); i++)
                values.append(parts[i]);
            insert(key, values);
        }
        file.close();
        return true;
    }

private:
    void insertNonFull(BPlusNode* node, Record r) {
        int i = node->numKeys - 1;
        if (node->isLeaf) {
            while (i >= 0 && r.key < node->records[i].key) {
                node->records[i + 1] = node->records[i];
                node->keys[i + 1] = node->keys[i];
                i--;
            }
            node->records[i + 1] = r;
            node->keys[i + 1] = r.key;
            node->numKeys++;
        } else {
            while (i >= 0 && r.key < node->keys[i]) i--;
            i++;
            if (node->children[i]->numKeys == ORDER) {
                splitChild(node, i, node->children[i]);
                if (r.key > node->keys[i]) i++;
            }
            insertNonFull(node->children[i], r);
        }
    }

    void splitChild(BPlusNode* parent, int i, BPlusNode* child) {
        int mid = ORDER / 2;
        BPlusNode* newNode = new BPlusNode(child->isLeaf);
        if (child->isLeaf) {
            for (int j = mid; j < child->numKeys; j++) {
                newNode->records[j - mid] = child->records[j];
                newNode->keys[j - mid] = child->keys[j];
            }
            newNode->numKeys = child->numKeys - mid;
            child->numKeys = mid;
            newNode->next = child->next;
            child->next = newNode;
            parent->keys[i] = newNode->keys[0];
        } else {
            for (int j = mid + 1; j < child->numKeys; j++) {
                newNode->keys[j - mid - 1] = child->keys[j];
                newNode->children[j - mid - 1] = child->children[j];
            }
            newNode->children[child->numKeys - mid - 1] = child->children[child->numKeys];
            newNode->numKeys = child->numKeys - mid - 1;
            parent->keys[i] = child->keys[mid];
            child->numKeys = mid;
        }
        for (int j = parent->numKeys; j > i; j--) {
            parent->children[j + 1] = parent->children[j];
            parent->keys[j] = parent->keys[j - 1];
        }
        parent->children[i + 1] = newNode;
        parent->numKeys++;
    }

    bool removeFromNode(BPlusNode* node, int key) {
        if (!node) return false;
        if (node->isLeaf) {
            for (int i = 0; i < node->numKeys; i++) {
                if (node->records[i].key == key) {
                    for (int j = i; j < node->numKeys - 1; j++) {
                        node->records[j] = node->records[j + 1];
                        node->keys[j] = node->keys[j + 1];
                    }
                    node->numKeys--;
                    return true;
                }
            }
            return false;
        }
        int i = 0;
        while (i < node->numKeys && key >= node->keys[i]) i++;
        return removeFromNode(node->children[i], key);
    }
};

#endif