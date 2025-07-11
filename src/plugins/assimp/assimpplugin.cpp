// SPDX-FileCopyrightText: 2024 Joshua Goins <josh@redstrate.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include <KPluginFactory>
#include <konvexplugininterface.h>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <QQuick3DGeometry>
#include <qvectornd.h>

class AssimpGeometry : public QQuick3DGeometry
{
    Q_OBJECT
    Q_PROPERTY(QVector3D minBound READ minBound CONSTANT)
    Q_PROPERTY(QVector3D maxBound READ maxBound CONSTANT)
    Q_PROPERTY(QVector3D center READ center CONSTANT)
    Q_PROPERTY(QVector3D extents READ extents CONSTANT)
public:
    QVector3D minBound() const { return m_minBound; }
    QVector3D maxBound() const { return m_maxBound; }
    QVector3D center() const { return m_center; }
    QVector3D extents() const { return m_extents; }

    void setProperty(const char* name, const QVector3D& value) {
        if (qstrcmp(name, "minBound") == 0) m_minBound = value;
        else if (qstrcmp(name, "maxBound") == 0) m_maxBound = value;
        else if (qstrcmp(name, "center") == 0) m_center = value;
        else if (qstrcmp(name, "extents") == 0) m_extents = value;
    }

private:
    QVector3D m_minBound;
    QVector3D m_maxBound;
    QVector3D m_center;
    QVector3D m_extents;

public:
    explicit AssimpGeometry(QQuick3DObject *parent)
        : QQuick3DGeometry(parent)
    {
    }

    ~AssimpGeometry() override
    {
        qInfo() << parent();
    }

    void setVertexData(const QByteArray &positions, const QByteArray &indices)
    {
        setStride(sizeof(float) * 3);
        setPrimitiveType(PrimitiveType::Triangles);
        addAttribute(Attribute::PositionSemantic, 0, Attribute::F32Type);
        addAttribute(Attribute::IndexSemantic, 0, Attribute::U32Type);
        QQuick3DGeometry::setVertexData(positions);
        QQuick3DGeometry::setIndexData(indices);
        update();
    }

private:
    QList<QVector3D> m_vertexData;
};

class AssimpPlugin : public KonvexPluginInterface
{
public:
    AssimpPlugin(QObject *parent, const QVariantList &args)
        : KonvexPluginInterface(parent)
    {
    }

    QQuick3DGeometry *createGeometry(const QString &url, QQuick3DObject *parent) override
    {
        Assimp::Importer importer;
        const aiScene *scene = importer.ReadFile(url.toStdString().c_str(),
                                                 aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType);
        if (nullptr == scene) {
            return nullptr;
        }

        auto geometry = new AssimpGeometry(parent);

        QByteArray positions;
        QByteArray indices;
 
        float minX = std::numeric_limits<float>::max();
        float minY = std::numeric_limits<float>::max();
        float minZ = std::numeric_limits<float>::max();
        float maxX = std::numeric_limits<float>::min();
        float maxY = std::numeric_limits<float>::min();
        float maxZ = std::numeric_limits<float>::min();
        
        bool hasBounds = false;

        
        const auto writeFloat = [&positions](const float value) {
            positions.append(reinterpret_cast<const char *>(&value), sizeof(float));
        };

        const auto writeUint = [&indices](const unsigned int value) {
            indices.append(reinterpret_cast<const char *>(&value), sizeof(unsigned int));
        };

        const auto updateBounds = [&](const aiVector3D &vertex)
        {
            minX = std::min(minX, vertex.x);
            minY = std::min(minY, vertex.y);
            minZ = std::min(minZ, vertex.z);
            maxX = std::max(maxX, vertex.x);
            maxY = std::max(maxY, vertex.y);
            maxZ = std::max(maxZ, vertex.z);
            hasBounds = true;
        };

        for (int i = 0; i < scene->mNumMeshes; i++) {
            auto mesh = scene->mMeshes[i];
            for (int j = 0; j < mesh->mNumVertices; j++) {
                auto vertex = mesh->mVertices[j];

                updateBounds(vertex);

                writeFloat(vertex.x);
                writeFloat(vertex.y);
                writeFloat(vertex.z);
            }

            for (int j = 0; j < mesh->mNumFaces; j++) {
                for (int z = 0; z < mesh->mFaces[j].mNumIndices; z++) {
                    writeUint(mesh->mFaces[j].mIndices[z]);
                }
            }
        }

        geometry->setVertexData(positions, indices);

        if (hasBounds)
        {
            geometry->setProperty("minBound", QVector3D(minX, minY, minZ));
            geometry->setProperty("maxBound", QVector3D(maxX, maxY, maxZ));

            const float centerX = (minX + maxX) / 2.0f;
            const float centerY = (minY + maxY) / 2.0f;
            const float centerZ = (minZ + maxZ) / 2.0f;
            geometry->setProperty("center", QVector3D(centerX, centerY, centerZ));

            const float extentX = (maxX - minX);
            const float extentY = (maxY - minY);
            const float extentZ = (maxZ - minZ);
            geometry->setProperty("extents", QVector3D(extentX, extentY, extentZ));
        }

        return geometry;
    }



};

K_PLUGIN_CLASS_WITH_JSON(AssimpPlugin, "assimpplugin.json")

#include "assimpplugin.moc"
