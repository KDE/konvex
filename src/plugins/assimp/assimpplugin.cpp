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

        const auto writeFloat = [&positions](const float value) {
            positions.append(reinterpret_cast<const char *>(&value), sizeof(float));
        };

        const auto writeUint = [&indices](const unsigned int value) {
            indices.append(reinterpret_cast<const char *>(&value), sizeof(unsigned int));
        };

        for (int i = 0; i < scene->mNumMeshes; i++) {
            auto mesh = scene->mMeshes[i];
            for (int j = 0; j < mesh->mNumVertices; j++) {
                auto vertex = mesh->mVertices[j];
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

        return geometry;
    }
};

K_PLUGIN_CLASS_WITH_JSON(AssimpPlugin, "assimpplugin.json")

#include "assimpplugin.moc"
